//
// Created by scott on 8/4/20.
//

#include "SecKeyManagement.h"


void CommandSecKeyNegotiateRequest::marshall( ByteStream& stream){
    CommandSecKeyBase::marshall(stream);
    len = 5 + key_data.size();
    stream.writeUnsignedInt(this->len);
    stream.writeByte(algorithm_id);
    stream.writeUnsignedInt(this->key_len);

    stream.writeBytes(this->key_data.data(),this->key_len);
}

CommandSecKeyNegotiateRequest::Ptr CommandSecKeyNegotiateRequest::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSecKeyNegotiateRequest>();
    cp->readHead(stream);
    cp->len = stream.readUnsignedInt();
    cp->algorithm_id = stream.readByte();
    cp->key_len = stream.readByte();
    cp->key_data.resize(cp->key_len);
    stream.readBytes(cp->key_data.data(),cp->key_len);

    return cp;
}

void CommandSecKeyNegotiateResponse::marshall( ByteStream& stream){
    CommandSecKeyBase::marshall(stream);
    len = 8;
    stream.writeUnsignedInt(this->len);
    stream.writeUnsignedInt(this->data);
}

CommandSecKeyNegotiateResponse::Ptr CommandSecKeyNegotiateResponse::parse(ByteStream& stream){
    auto  cp = std::make_shared<CommandSecKeyNegotiateResponse>();
    cp->readHead(stream);
    cp->len = stream.readUnsignedInt();
    cp->data = stream.readUnsignedInt();
    return cp;
}

///

void CommandSecKeyDistributeRequest::marshall( ByteStream& stream){
    CommandSecKeyBase::marshall(stream);
    len = 4;
    for(auto & key: dist_Keys){
        len+=key.getSize();
    }

    stream.writeUInt(this->len);
    stream.writeUInt(this->key_num);
//    stream.writeBytes(this->data.data(),this->data.size());
    for(auto & key : dist_Keys){
        key.marshall(stream);
    }
}

CommandSecKeyDistributeRequest::Ptr CommandSecKeyDistributeRequest::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSecKeyDistributeRequest>();
    cp->readHead(stream);
    cp->len = stream.readUInt();
    cp->key_num = stream.readUInt();

    for(int n=0; n< cp->key_num ;n ++){
        SecKeyBody body;
        body.parse(stream);
        cp->dist_Keys.push_back(body);
    }
//    cp->data.resize(cp->key_num);
//    stream.readBytes(cp->data.data(),cp->)
    return cp;
}

void CommandSecKeyDistributeResponse::marshall( ByteStream& stream){
    CommandSecKeyBase::marshall(stream);
    stream.writeUInt(len);
    stream.writeUInt(result);
    for(auto& id: dist_keys){
        id.marshall(stream);
    }
}

CommandSecKeyDistributeResponse::Ptr CommandSecKeyDistributeResponse::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSecKeyDistributeResponse>();
    cp->readHead(stream);
    cp->len = stream.readUInt();
    cp->result = stream.readUInt();
    if(cp->result > 0){
        for(size_t n=0; n< cp->result ;n++){
            AtomKeyID id;
            id.parse(stream);
            cp->dist_keys.push_back(id);
        }
    }
    return CommandSecKeyDistributeResponse::Ptr();
}

/// CommandSecKeyDestroyRequest
void CommandSecKeyDestroyRequest::marshall( ByteStream& stream){
    CommandSecKeyBase::marshall(stream);


}

CommandSecKeyDestroyRequest::Ptr CommandSecKeyDestroyRequest::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSecKeyDestroyRequest>();
    cp->readHead(stream);
    return cp;
}


/// CommandSecKeyDestroyResponse

void CommandSecKeyDestroyResponse::marshall( ByteStream& stream){
    len = 4;
    stream.writeUInt( len);
    stream.writeUInt(result);
}

CommandSecKeyDestroyResponse::Ptr CommandSecKeyDestroyResponse::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSecKeyDestroyResponse>();
    cp->readHead(stream);
    cp->len = stream.readUInt();
    cp->result = stream.readUInt();
    return cp;
}

/// CommandSecKeyEnableRequest

CommandSecKeyEnableRequest::Ptr CommandSecKeyEnableRequest::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSecKeyEnableRequest>();
    cp->readHead(stream);
    cp->len = stream.readUInt();
    cp->flag = stream.readByte();
    for(size_t n=0 ;n< (cp->len-1)/4; n++){
        std::uint32_t  key_id;
        key_id = stream.readUInt();
        cp->key_ids.push_back(key_id);
    }
    return cp;
}

void CommandSecKeyEnableRequest::marshall( ByteStream& stream){
    CommandSecKeyBase::marshall(stream);
    stream.writeUInt(len);
    stream.writeByte(flag);
    for(auto id : key_ids){
        stream.writeUInt( id );
    }
}

/// CommandSecKeyEnableResponse
void CommandSecKeyEnableResponse::marshall( ByteStream& stream){
    CommandSecKeyBase::marshall(stream);
    len = 4 + key_ids.size() * 4;

    stream.writeUInt(len);
    stream.writeUInt(result);
    for(auto id: key_ids){
        stream.writeUInt(id);
    }
}

CommandSecKeyEnableResponse::Ptr CommandSecKeyEnableResponse::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSecKeyEnableResponse>();
    cp->readHead(stream);
    cp->len = stream.readUInt();
    cp->result = stream.readUInt();
    if(cp->result) {
        for (size_t n = 0; n < cp->result; n++) {
            std::uint32_t key_id;
            key_id = stream.readUInt();
            cp->key_ids.push_back(key_id);
        }
    }
    return cp;
}

/// CommandSecKeyApplyRequest
void CommandSecKeyApplyRequest::marshall( ByteStream& stream){
    CommandSecKeyBase::marshall(stream);
    len = 8;
    stream.writeUInt(len);
    stream.writeUInt(key_type);
    stream.writeUInt(key_num);
}

CommandSecKeyApplyRequest::Ptr CommandSecKeyApplyRequest::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSecKeyApplyRequest>();
    cp->readHead(stream);
    cp->len = stream.readUInt();
    cp->key_type = stream.readUInt();
    cp->key_num = stream.readUInt();
    return CommandSecKeyApplyRequest::Ptr();
}

/// CommandSecKeyApplyResponse
void CommandSecKeyApplyResponse::marshall( ByteStream& stream){
    CommandSecKeyBase::marshall(stream);
    len = 12;
    for(auto & key: key_list){
        len+=key.getSize();
    }
    stream.writeUInt(len);
    stream.writeUInt(result);
    stream.writeUInt(key_num);
    for(auto& key: key_list){
        key.marshall(stream);
    }
}

CommandSecKeyApplyResponse::Ptr CommandSecKeyApplyResponse::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSecKeyApplyResponse>();
    cp->readHead(stream);
    cp->len = stream.readUInt();
    cp->result = stream.readUInt();
    cp->key_num = stream.readUInt();
    for(size_t n=0 ; n< cp->key_num ;n++){
        SecKeyBody key;
        key.parse(stream);
        cp->key_list.push_back(key);
    }
    return cp;
}





