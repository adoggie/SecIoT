//
// Created by scott on 8/4/20.
//

#include "ComplianceTestImpl.h"
#include "../GM50/apps/ComplianceTest.h"
#include "../GM50/sechannel.h"
#include "../sdf-utils.h"

void ComplianceTestImpl::onCommand(const Command::Ptr& command){
    {
        CommandComplianceTestRequest::Ptr cp = std::dynamic_pointer_cast<CommandComplianceTestRequest>(command);
        if(cp.get()){
            onCommandComplianceTestRequest(cp);
            return;
        }
    }

    {
        CommandComplianceTestResponse::Ptr cp = std::dynamic_pointer_cast<CommandComplianceTestResponse>(command);
        if(cp.get()){
            onCommandComplianceTestResponse(cp);
            return;
        }
    }

//    this->channel;

}

/// gm53/6.2.3.1.1

#define ALGORITHM_SYMM 0
#define ALGORITHM_ASYMM 1
#define ALGORITHM_HASH 2

#define CODEC_MODE_ECB 0
#define CODEC_MODE_CBC 1
#define CODEC_MODE_CFB 2
#define CODEC_MODE_OFB 3

#define CODEC_MODE_RSA 0
#define CODEC_MODE_SM2 1
#define CODEC_MODE_HASH 0

#define COMPOSE_FLAG_PLAIN 0
#define COMPOSE_FLAG_PLAIN_AND_KEY 1
#define COMPOSE_FLAG_CYPHER_AND_KEY 2

#define COMPOSE_FLAG_PubKey_Plain 0
#define COMPOSE_FLAG_PriKey_Envelope 1
#define COMPOSE_FLAG_PriKey_Plain 2
#define COMPOSE_FLAG_PubKey_Sign 3
#define COMPOSE_FLAG_PubKey_TempPubKey_SenderId  4

#define COMPOSE_FLAG_BigThanGroup  1
#define COMPOSE_FLAG_LessThanGroup  2

void ComplianceTestImpl::onCommandComplianceTestRequest(const CommandComplianceTestRequest::Ptr& command){


    // gm30/6.2.3.1.2/a
    if ( command->schema == ComplianceSchemaDetail(0,0,0,0) || // ECB
        command->schema == ComplianceSchemaDetail(0,1,0,0) || // CBC
        command->schema == ComplianceSchemaDetail(0,2,0,0) || // CFB
        command->schema == ComplianceSchemaDetail(0,3,0,0) ){ // OFB

        ByteStream stream(command->data.data(),command->data.size());
        std::uint32_t  algo_id = stream.readUInt();
        std::uint32_t  plaintext_len = stream.readUInt();
        ByteArray plain;
        plain.resize(plaintext_len);
        stream.readBytes(plain.data(),plain.size());
        this->onSymmetricEncrypt(command->schema,algo_id,plain);
        return ;
    }

    // gm30/6.2.3.1.2/b  symmetric encrypt 2
    // plain and cipher -> cipher text
    if ( command->schema == ComplianceSchemaDetail(0,0,0,1) || // ECB
         command->schema == ComplianceSchemaDetail(0,1,0,1) || // CBC
         command->schema == ComplianceSchemaDetail(0,2,0,1) || // CFB
         command->schema == ComplianceSchemaDetail(0,3,0,1) ){ // OFB

        ByteStream stream(command->data.data(),command->data.size());
        std::uint32_t  algo_id = stream.readUInt();
        std::uint32_t  cipher_len = stream.readUInt();
        ByteArray cipher;
        cipher.resize(cipher_len);
        stream.readBytes(cipher.data(),cipher.size());
        std::uint32_t  plain_len = stream.readUInt();

        ByteArray  plain;
        plain.resize(plain_len);
        stream.readBytes(plain.data(),plain.size());
        this->onSymmetricEncrypt2(command->schema,algo_id,plain,cipher);
        return ;
    }

    // gm30/6.2.3.1.2/c  symmetric decrypt 2
    // cipherText and cipher ->  plain text
    if ( command->schema == ComplianceSchemaDetail(0,0,1,2) || // ECB
         command->schema == ComplianceSchemaDetail(0,1,1,2) || // CBC
         command->schema == ComplianceSchemaDetail(0,2,1,2) || // CFB
         command->schema == ComplianceSchemaDetail(0,3,1,2) ){ // OFB

        ByteStream stream(command->data.data(),command->data.size());
        std::uint32_t  algo_id = stream.readUInt();
        std::uint32_t  cipher_len = stream.readUInt();
        ByteArray cipher;
        cipher.resize(cipher_len);
        stream.readBytes(cipher.data(),cipher.size());

        std::uint32_t  cipher_text_len = stream.readUInt();

        ByteArray  cipher_text;
        cipher_text.resize(cipher_text_len);
        stream.readBytes(cipher_text.data(),cipher_text.size());
        this->onSymmetricDecrypt(command->schema,algo_id,cipher,cipher_text);
        return ;
    }

    // gm30/6.2.3.1.3/a  asymmetric encrypt
    // pubkey and plain text ->  envelope
    if ( command->schema == ComplianceSchemaDetail(1,0,0,0) || // RSA
         command->schema == ComplianceSchemaDetail(1,1,0,0)) { // SM2

        ByteStream stream(command->data.data(),command->data.size());
        std::uint32_t  algo_id = stream.readUInt();
        std::uint32_t  pubkey_len = stream.readUInt();
        ByteArray pubkey;
        pubkey.resize(pubkey_len);
        stream.readBytes(pubkey.data(),pubkey.size());

        std::uint32_t  plain_len = stream.readUInt();

        ByteArray  plain;
        plain.resize(plain_len);
        stream.readBytes(plain.data(),plain.size());
        this->onASymmetricEncrypt(command->schema,algo_id,pubkey,plain);
        return ;
    }

    // gm30/6.2.3.1.3/b asymmetric decrypt
    // pubkey and plain text ->  envelope
    if ( command->schema == ComplianceSchemaDetail(1,0,1,1) || // RSA
         command->schema == ComplianceSchemaDetail(1,1,1,1)) { // SM2

        ByteStream stream(command->data.data(),command->data.size());
        std::uint32_t  algo_id = stream.readUInt();
        std::uint32_t  prikey_len = stream.readUInt();
        ByteArray prikey;
        prikey.resize(prikey_len);
        stream.readBytes(prikey.data(),prikey.size());

        std::uint32_t  evp_len = stream.readUInt();

        ByteArray  evp;
        evp.resize(evp_len);
        stream.readBytes(evp.data(),evp.size());
        this->onASymmetricDecrypt(command->schema,algo_id,prikey,evp);
        return ;
    }


    // gm30/6.2.3.1.3/c asymmetric signature
    // prikey and plain text ->  sign_data
    if ( command->schema == ComplianceSchemaDetail(1,0,2,0) || // RSA
         command->schema == ComplianceSchemaDetail(1,1,2,0)) { // SM2

        ByteStream stream(command->data.data(),command->data.size());
        std::uint32_t  algo_id = stream.readUInt();
        std::uint32_t  prikey_len = stream.readUInt();
        ByteArray prikey;
        prikey.resize(prikey_len);
        stream.readBytes(prikey.data(),prikey.size());

        std::uint32_t  plain_len = stream.readUInt();

        ByteArray  plain;
        plain.resize(plain_len);
        stream.readBytes(plain.data(),plain.size());
        this->onASymmetricSign(command->schema,algo_id,prikey,plain);
        return ;
    }

    // gm30/6.2.3.1.3/d asymmetric verify
    // pubkey and sign data ->  plain text
    if ( command->schema == ComplianceSchemaDetail(1,0,3,0) || // RSA
         command->schema == ComplianceSchemaDetail(1,1,3,0)) { // SM2

        ByteStream stream(command->data.data(),command->data.size());
        std::uint32_t  algo_id = stream.readUInt();
        std::uint32_t  pubkey_len = stream.readUInt();
        ByteArray pubkey;
        pubkey.resize(pubkey_len);
        stream.readBytes(pubkey.data(),pubkey.size());

        std::uint32_t  sign_len = stream.readUInt();

        ByteArray  sign;
        sign.resize(sign_len);
        stream.readBytes(sign.data(),sign.size());
        this->onASymmetricVerify(command->schema,algo_id,pubkey,sign);
        return ;
    }

    // gm30/6.2.3.1.3/e  key exchange
    // pubkey and temp pubkey and sender id ->  pubkey , temp pubkey and response id
    if ( command->schema == ComplianceSchemaDetail(1,0,4,0) || // RSA
         command->schema == ComplianceSchemaDetail(1,1,4,0)) { // SM2

        ByteStream stream(command->data.data(),command->data.size());
        std::uint32_t  algo_id = stream.readUInt();
        std::uint32_t  pubkey_len = stream.readUInt();
        ByteArray pubkey;
        pubkey.resize(pubkey_len);
        stream.readBytes(pubkey.data(),pubkey.size());

        std::uint32_t  temp_pubkey_len = stream.readUInt();
        ByteArray  temp_pubkey;
        temp_pubkey.resize(temp_pubkey_len);
        stream.readBytes(temp_pubkey.data(),temp_pubkey.size());

        std::uint32_t  sender_id;
        sender_id = stream.readUInt();

        this->onExchangeKey(command->schema,algo_id,pubkey,temp_pubkey, sender_id);
        return ;
    }

    // gm30/6.2.3.1.4  hash check
    // plain text -> hash
    if ( command->schema == ComplianceSchemaDetail(2,0,0,1) || // RSA
         command->schema == ComplianceSchemaDetail(2,0,0,2)) { // SM2

        ByteStream stream(command->data.data(),command->data.size());
        std::uint32_t  algo_id = stream.readUInt();
        std::uint32_t  plain_len = stream.readUInt();
        ByteArray plain;
        plain.resize(plain_len);
        stream.readBytes(plain.data(),plain.size());
        this->onHashCheck(command->schema,algo_id,plain);
        return ;
    }

    if( command->data.size() == 0){
        // deivce self check
        std::uint32_t  content;
        content = command->len;
        this->onDeviceSelfCheck(content);
        return;
    }
}


void ComplianceTestImpl::onCommandComplianceTestResponse(const CommandComplianceTestResponse::Ptr& command){
    // do nothing
}

// plain -> cipher_text and cipher
// gm53/6.2.3.1.2/a
void ComplianceTestImpl::onSymmetricEncrypt(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray & data){

    ByteArray cipher;
    ByteArray  cipherText;
    sdf::random_create(cipher,32);

    sdf::data_enc(sdf::KeyID(0),algo_id,cipher.data(),cipher.size(),data.data(),data.size(),cipherText);

    auto cp = std::make_shared<CommandComplianceTestResponse>();

    ByteStream response;
    cp->schema = schema;
    std::uint32_t schema_len;
    schema_len = 12 + cipher.size() + cipherText.size();
    response.writeUInt( schema_len);
    response.writeUInt(algo_id);
    response.writeUInt(cipher.size());
    response.writeBytes(cipher.data(),cipher.size());
    response.writeUInt(cipherText.size());
    response.writeBytes(cipherText.data(),cipherText.size());
    cp->data.assign(response.data(),response.data()+response.size());
    this->channel->sendCommand(cp);
}

// plain and cipher -> cipher text
// gm53/6.2.3.1.2/b
void ComplianceTestImpl::onSymmetricEncrypt2(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray & plain, const ByteArray& cipher){
    ByteArray  cipherText;
    sdf::data_enc(sdf::KeyID(0),algo_id,cipher.data(),cipher.size(),plain.data(),plain.size(),cipherText);
    auto cp = std::make_shared<CommandComplianceTestResponse>();

    ByteStream response;
    cp->schema = schema;
    std::uint32_t schema_len;
    schema_len = 12 + cipher.size() + cipherText.size();
    response.writeUInt( schema_len);
    response.writeUInt(algo_id);
    response.writeUInt(cipherText.size());
    response.writeBytes(cipherText.data(),cipherText.size());
    cp->data.assign(response.data(),response.data()+response.size());
    this->channel->sendCommand(cp);
}

// gm53/p13 / c)
void ComplianceTestImpl::onSymmetricDecrypt(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray & cipher,const ByteArray & cipher_text){
    ByteArray  plainText;
    sdf::data_dec(sdf::KeyID(0),algo_id,cipher.data(),cipher.size(),cipher_text.data(),cipher_text.size(),plainText);

    auto cp = std::make_shared<CommandComplianceTestResponse>();

    ByteStream response;
    cp->schema = schema;
    std::uint32_t schema_len;
    schema_len = 8 + plainText.size();
    response.writeUInt( schema_len); // write stream from schema_len
    response.writeUInt(algo_id);
    response.writeUInt(plainText.size());
    response.writeBytes(plainText.data(),plainText.size());
    cp->data.assign(response.data(),response.data()+response.size());
    this->channel->sendCommand(cp);
}


///gm53/6.2.3.2.2.3/ a)
void ComplianceTestImpl::onASymmetricEncrypt(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray& pub_key,const ByteArray & data){
    ByteArray  cipherText;
    sdf::data_enc(sdf::KeyID(0),algo_id,pub_key.data(),pub_key.size(),data.data(),data.size(),cipherText);

    auto cp = std::make_shared<CommandComplianceTestResponse>();
    ByteStream envelope;
    ByteStream response;
    cp->schema = schema;
    std::uint32_t schema_len;
    schema_len = 8 + envelope.size();
    response.writeUInt( schema_len); // write stream from schema_len
    response.writeUInt(algo_id);
    response.writeUInt(envelope.size());
    response.writeBytes(envelope.data(),envelope.size());
    cp->data.assign(response.data(),response.data()+response.size());
    this->channel->sendCommand(cp);
}

// decode from envelope to plain text
void ComplianceTestImpl::onASymmetricDecrypt(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray & priv_key,const ByteArray & envelope){
    ByteArray  plainText;

    sdf::data_dec(sdf::KeyID(0),algo_id,priv_key.data(),priv_key.size(),envelope.data(),envelope.size(),plainText);

    auto cp = std::make_shared<CommandComplianceTestResponse>();

    ByteStream response;
    cp->schema = schema;
    std::uint32_t schema_len;
    schema_len = 8 + plainText.size();
    response.writeUInt( schema_len); // write stream from schema_len
    response.writeUInt(algo_id);
    response.writeUInt(plainText.size());
    response.writeBytes(plainText.data(),plainText.size());
    cp->data.assign(response.data(),response.data()+response.size());
    this->channel->sendCommand(cp);
}

//gm53/p15 /c
void ComplianceTestImpl::onASymmetricSign(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray& priv_key,const ByteArray & data){
    ByteArray  signText;
    sdf::data_sign(sdf::KeyID(0),algo_id,priv_key.data(),priv_key.size(),data.data(),data.size(),signText);

    auto cp = std::make_shared<CommandComplianceTestResponse>();
//    ByteStream envelope;
    ByteStream response;
    cp->schema = schema;
    std::uint32_t schema_len;
    schema_len = 8 + signText.size();
    response.writeUInt( schema_len); // write stream from schema_len
    response.writeUInt(algo_id);
    response.writeUInt(signText.size());
    response.writeBytes(signText.data(),signText.size());
    cp->data.assign(response.data(),response.data()+response.size());
    this->channel->sendCommand(cp);
}

// gm53/p15 / d
void ComplianceTestImpl::onASymmetricVerify(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray & pub_key,const ByteArray & sign_val){
//    ByteArray  signText;
//    sdf::data_verify(sdf::KeyID(0),algo_id,pub_key.data(),pub_key.size(),sign_val.data(),sign_val.size());
//
//    auto cp = std::make_shared<CommandComplianceTestResponse>();
//    ByteStream response;
//    cp->schema = schema;
//    std::uint32_t schema_len;
//    schema_len = 8 + signText.size();
//    response.writeUInt( schema_len); // write stream from schema_len
//    response.writeUInt(algo_id);
//    response.writeUInt(signText.size());
//    response.writeBytes(signText.data(),signText.size());
//    cp->data.assign(response.data(),response.data()+response.size());
//    this->channel->sendCommand(cp);
}

// 6.2.3.1.3  e)
//    void onExchangeKey();
void ComplianceTestImpl::onHashCheck(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,ByteArray& data){
    ByteArray  hash;
    sdf::data_hash(algo_id,data.data(),data.size(),hash);

    auto cp = std::make_shared<CommandComplianceTestResponse>();
    ByteStream response;
    cp->schema = schema;
    std::uint32_t schema_len;
    schema_len = 8 + hash.size();
    response.writeUInt( schema_len); // write stream from schema_len
    response.writeUInt(algo_id);
    response.writeUInt(hash.size());
    response.writeBytes(hash.data(),hash.size());
    cp->data.assign(response.data(),response.data()+response.size());
    this->channel->sendCommand(cp);
}

///gm53/p11
void ComplianceTestImpl::onExchangeKey(const ComplianceSchemaDetail& schema,std::uint32_t algo_id, const ByteArray& pub_key, const ByteArray& temp_pub_key, uint32_t sender_id){
//    ByteArray  hash;
//    sdf::data_hash(algo_id,data.data(),data.size(),hash);
//
//    auto cp = std::make_shared<CommandComplianceTestResponse>();
//    ByteStream response;
//    cp->schema = schema;
//    std::uint32_t schema_len;
//    schema_len = 8 + hash.size();
//    response.writeUInt( schema_len); // write stream from schema_len
//    response.writeUInt(algo_id);
//    response.writeUInt(hash.size());
//    response.writeBytes(hash.data(),hash.size());
//    cp->data.assign(response.data(),response.data()+response.size());
//    this->channel->sendCommand(cp);
}


void ComplianceTestImpl::onDeviceSelfCheck(std::uint32_t content){
    ByteArray  hash;

    auto cp = std::make_shared<CommandComplianceTestResponse>();
    ByteStream response;
    cp->schema = ComplianceSchemaDetail(3,0,0,0) ;
    std::uint32_t schema_len;
    std::uint32_t  result = 0;
    schema_len = 8 + hash.size();
    response.writeUInt( schema_len); // write stream from schema_len
    response.writeUInt(result);
    this->channel->sendCommand(cp);
}