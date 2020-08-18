//
// Created by scott on 7/29/20.
//
#include "message.h"
#include "command.h"


#include "sechannel.h"
#include "../sdf-utils.h"
#include "gm0006.h"

#include <map>



MessageChannelTraverse::Ptr MessageTraverseParser::parse(ByteStream& stream){
//    Json::Reader reader;
//    Json::Value root;
//    Message::Ptr msg;
//    if (reader.parse(data, root)){
//        for(auto func:parse_func_list){
//            msg = func(root);
//            if(msg){
//                msg->id_ = root["id"].asString();
//                break;
//            }
//        }
//    }
//    return msg;
    return MessageChannelTraverse::Ptr();
}

// GMT50/6.2.2
std::size_t  MessageChannelCreateRequest::getSize() const{
    return enc_data.size() + sign_data.size() + 8;
}

MessageChannelCreateRequest::Ptr MessageChannelCreateRequest::parse( ByteStream& stream){
    MessageChannelCreateRequest::Ptr m = std::make_shared<MessageChannelCreateRequest>();
    m->enc_len = stream.readUInt();
    m->enc_data.resize(m->enc_len);
    stream.readBytes((char*)&m->enc_data[0],m->enc_len);

    m->sign_len = stream.readUInt();
    m->sign_data.resize(m->sign_len);
    stream.readBytes((char*)&m->sign_data[0],m->sign_len);
    return m;
}
void MessageChannelCreateRequest::marshall(ByteStream& stream){
    stream.writeUInt( this->enc_data.size());
    stream.writeBytes(this->enc_data.data(), this->enc_data.size());
    stream.writeUInt(this->sign_data.size());
    stream.writeBytes(this->sign_data.data(),this->sign_data.size());

}

/// GM50/6.2.3
MessageChannelCreateResponse::Ptr MessageChannelCreateResponse::parse(ByteStream& stream){
    MessageChannelCreateResponse::Ptr m = std::make_shared<MessageChannelCreateResponse>();
    m->enc_len = stream.readUInt();
    m->enc_data.resize(m->enc_len);
    stream.readBytes((char*)&m->enc_data[0],m->enc_len);

    m->sign_len = stream.readUInt();
    m->sign_data.resize(m->sign_len);
    stream.readBytes((char*)&m->sign_data[0],m->sign_len);
    return m;
}

void MessageChannelCreateResponse::marshall(ByteStream& stream){
    stream.writeUInt( this->enc_data.size());
    stream.writeBytes(this->enc_data.data(), this->enc_data.size());
    stream.writeUInt(this->sign_data.size());
    stream.writeBytes(this->sign_data.data(),this->sign_data.size());
}

/// GM50/8.1
MessageChannelSend::Ptr MessageChannelSend::parse(ByteStream& stream){
    MessageChannelSend::Ptr  m = std::make_shared<MessageChannelSend>();
    std::uint8_t  app_id = stream.readByte();
    std::uint8_t  type = stream.readByte();
    stream.next(-1);
    for(auto &  p : CommandParseFuncList){
        if( p.first == CommandType(type)){
            Command::Ptr  cmd = p.second(stream);
            m->detail = cmd;
            break;
        }
    }
    return m;
}

void MessageChannelSend::marshall(ByteStream& stream){
    ByteStream pdu;
    this->detail->marshall(pdu );

    ByteArray key = this->channel->getTempKey(); // 远端peer 的会话密钥
    ByteArray cipherText;
    key.data();
    pdu.data();
    sdf::data_enc(sdf::KeyID(0), SGD_SM4_ECB, key.data(), key.size(), pdu.data(), pdu.size(), cipherText);
    stream.writeBytes(cipherText.data(), cipherText.size());
}


MessageChannelReCreateRequest::Ptr MessageChannelReCreateRequest::parse(ByteStream& stream){
    // must be null
    return std::make_shared<MessageChannelReCreateRequest>();
}

void MessageChannelReCreateRequest::marshall(ByteStream& stream){

}