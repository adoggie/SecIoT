//
// Created by scott on 7/29/20.
//

#include "envelope.h"
#include "bytearray.h"
#include "../sdf-utils.h"
#include "gm0006.h"
#include "sechannel.h"

#define PACKET_HDRSIZE 81
#define MAX_PACKET_SIZE 1024*1024*10

#define Okay 0
#define NeedMore 1
#define Dirty 2


void Envelope::marshall(ByteStream& stream){
    stream.writeByte(version);
    stream.writeByte(safe_mode);
    stream.writeShort(not_used);
    stream.writeUInt64(msg_id);

    ByteStream pdu;
    detail->marshall(pdu);
    pdu_len = pdu.size();

    stream.writeUInt(pdu_len);
    stream.writeBytes(target.data(),target.size());
    stream.writeBytes(source.data(),source.size());
    stream.writeByte((uint8_t) op_type);
    stream.writeBytes(pdu.data(),pdu.size());


//    if( pdu.size()) {
//        ByteArray &key = this->channel->getTempKey();
//        ByteArray cipherText;
//        sdf::data_enc(sdf::KeyID(0), SGD_SM4_ECB, key.data(), key.size(), pdu.data(), pdu.size(), cipherText);
//        stream.writeBytes(cipherText.data(), cipherText.size());
//    }
//
//    ByteArray hash;
//    sdf::data_hash(SGD_USER_HMAC,stream.data(),stream.size(),hash);

    ByteArray sign;

    auto& settings = this->channel->getSettings();
    sdf::data_sign(sdf::KeyID(0),SGD_SM2,settings.local_pri_key.data(),settings.local_pri_key.size(),
            stream.data(),stream.size(),sign);
    stream.writeUInt(sign.size());
    stream.writeBytes( sign.data(),sign.size());
}

void Envelope::finish(ByteStream& stream,ByteArray sign_data){
    stream.writeUInt( sign_data.size());
    stream.writeBytes( sign_data.data(), sign_data.size());
}

//MessageTraverse::Ptr  Envelope::getMessage(){
//    return MessageTraverse::Ptr ();
//}

///

int  EnvelopeManager::enqueue(const char * data, size_t size ,EnvelopeArray & evps){

    size_t consumed ;
    int error;

    std::vector< std::shared_ptr<ByteStream> > bulks;
    buff_.writeBytes(data,size);
    error = this->split(buff_.data(), buff_.size(),consumed, bulks );

    for(auto & bulk: bulks){
        Envelope::Ptr evp = this->parse( bulk->data() ,bulk->size());
        if( evp.get()){
            evps.push_back(evp);
        }else{
            return Dirty;
        }
    }

    if( error == Dirty){
        buff_.reset();
        return error;
    }else {
        if(consumed) {
            buff_.erase(0, consumed);
        }
    }
    return Okay;
}


int EnvelopeManager::split(const char * data ,size_t size , size_t & consumed, std::vector< std::shared_ptr<ByteStream> > & bulks ){

    consumed = 0 ;
    while(size > 0){
        if(size <= PACKET_HDRSIZE){
            return NeedMore;
        }
        ByteStream bytes(data,size);

        auto version = bytes.readByte();
        if( version != int(Version::Version) ){
            return Dirty;
        }

        if(size > MAX_PACKET_SIZE){
            return Dirty;
        }

        bytes.position(12);
        std::uint32_t  sign_len ;
        std::uint32_t  pdu_len = bytes.readUnsignedInt(); //
        if(size > (PACKET_HDRSIZE+pdu_len+4)){
            bytes.position( PACKET_HDRSIZE+pdu_len );
            sign_len = bytes.readUnsignedInt();
            size_t eaten =  (PACKET_HDRSIZE + pdu_len + 4 + sign_len);
            if( size >= eaten ){
                std::shared_ptr<ByteStream> bulk( new ByteStream( data, eaten ) );
                bulks.push_back(bulk);

                data += eaten;
                size -= eaten;
                consumed += eaten;
            }
        }else{
            return NeedMore;
        }
    }
    return NeedMore;
}

///

typedef std::function< std::shared_ptr<MessageChannelTraverse> (ByteStream& stream) > ParseFuncChannelTraverse;

std::vector<ParseFuncChannelTraverse> parse_func_list_channel_travese={
        MessageChannelCreateRequest::parse,
        MessageChannelCreateResponse::parse,
        MessageChannelSend::parse,
        MessageChannelReCreateRequest::parse,
};

std::map< OperationType, ParseFuncChannelTraverse> ChannelMessageParseFuncList = {
        { OperationType::ChannelCreateRequest , MessageChannelCreateRequest::parse},
        { OperationType::ChannelCreateResponse , MessageChannelCreateResponse::parse},
        { OperationType::ChannelMessageSend, MessageChannelSend::parse},
        { OperationType::ChannelReCreateRequest , MessageChannelReCreateRequest::parse},

};

Envelope::Ptr EnvelopeManager::parse(const char *  data ,size_t size){
    ByteStream stream(data,size);
    Envelope::Ptr  evp = std::make_shared<Envelope>();
    evp->version = stream.readByte();
    evp->safe_mode = stream.readByte();
    evp->not_used = stream.readShort();
    evp->msg_id = stream.readUInt64();
    evp->pdu_len = stream.readUnsignedInt();

    evp->target.resize(32);
    evp->source.resize(32);
    stream.readBytes( evp->target.data(), 32 );
    stream.readBytes( evp->source.data() , 32);
    evp->op_type =(OperationType) stream.readByte();

    evp->channel = channel_;
    for(auto& p: ChannelMessageParseFuncList){
        if( evp->op_type == p.first){
            ByteArray pdu(stream.current() , stream.current() + evp->pdu_len);
            if(evp->op_type == OperationType::ChannelMessageSend){
                //decrypt data
                ByteArray data;
                decryptPDU(pdu,data);
                pdu = data;
            }
            ByteStream pdustream(pdu.data(),pdu.size());
            MessageChannelTraverse::Ptr  message = p.second( pdustream );
            evp->detail = message;
            message->envelope = evp.get();
            break;
        }
    }
    stream.next( evp->pdu_len);
    // verify( header + pdu)
    std::uint32_t  sign_data_size = stream.position();


    evp->sign_len = stream.readUInt();
    evp->sign_data.resize(evp->sign_len);
    stream.readBytes((char*)&evp->sign_data[0],evp->sign_data.size());

    ByteArray remote_pub_key = channel_->getRemotePubkey( evp->source);

    if( !sdf::data_verify(sdf::KeyID(0),SGD_SM2,
                          remote_pub_key.data(),
                          remote_pub_key.size(),
                     stream.data(),sign_data_size,
                     evp->sign_data.data(),evp->sign_data.size()
    )){
        std::cout<< "Invalid Envelope ( data Sign error) " << std::endl;
        return Envelope::Ptr();
    }

//    this->channel_->getSettings().remote_id
    return evp;
}

void EnvelopeManager::decryptPDU(ByteArray& pdu ,ByteArray& data){
    ByteArray& key = this->channel_->getSettings().temp_key; // use my key
    sdf::data_dec(sdf::KeyID(0), SGD_SMS4_ECB,
                  key.data(), key.size(),
                  pdu.data(), pdu.size(),
                  data
    );
}

Envelope::Ptr EnvelopeManager::createEnvelope(){
    auto evp = std::make_shared<Envelope>();
    evp->version = std::uint8_t (Version::Version);
    evp->safe_mode = std::uint8_t (SafeModeField::All);
    evp->msg_id = generateNextSequence();
    evp->pdu_len = 0;

    evp->channel = this->channel_;
    return evp;
}

std::uint64_t  EnvelopeManager::generateNextSequence(){
    return sequence_++;
}


void EnvelopeManager::reset(){

}