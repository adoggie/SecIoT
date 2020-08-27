//
// Created by scott2 on 8/6/20.
//

#include <iterator>
#include <algorithm>

#include "sdf-utils.h"
//#include <sdf_random.h>
//#include <sdf_random.h>
//#include "sdf_sm3.h"
//#include "sdf_sm4.h"
//#include "sdf_sm2_private_key.h"
//#include "sdf_sm2_public_key.h"
//#include "sdf_interior.h"
#include "SDF/sdf.h"
#include "SDF/sdf_type.h"
#include "SDF/sdf_dev_manage.h"
#include "GM50/gm0006.h"


#include <algorithm>
#include <iterator>

namespace sdf {

#ifndef _SDF_IMPL

    bool init();
    void cleanup();

    void random_create(ByteArray &bytes,size_t size) {
        bytes.resize(size);
        std::fill_n(bytes.begin(),size,0xFF);
    }

    void data_enc(KeyID key_id,AlgorithmType algo_type,const char * key, size_t key_size, const char* data,size_t  size,ByteArray &enc_data) {
        enc_data.assign(data,data+size);
    }

    void data_dec(KeyID key_id,AlgorithmType algo_type,const char * key, size_t key_size, const char* enc_data,size_t  size,ByteArray & data) {
        data.assign(enc_data,enc_data+size);
    }

    void data_sign(KeyID key_id,AlgorithmType algo_type,const char * pri_key, size_t key_size, const char* data,size_t  size,ByteArray &enc_data) {
//        enc_data.assign(data,data+size);
        enc_data.resize(20);
        std::fill_n(std::back_inserter(enc_data),20,'A');
    }

    bool data_verify(KeyID key_id,AlgorithmType algo_type,const char * pub_key, size_t key_size,const char* hash_data,size_t hash_size, const char* sign_data,size_t  sign_size) {
        return true;
    }

    void data_hash(AlgorithmType algo_type, const char* data,size_t  size,ByteArray &hash) {
        std::fill_n(std::back_inserter(hash),20,0xff);

    }

    KeyID get_current_keyid(){
        return  0;
    }

    void  set_current_keyid(KeyID){

    }

    void  import_keys( std::vector<SecKeyBody>& keys){

    }

    void  destroy_keys(){


    }

    void  enable_keys(std::vector<std::uint32_t> keys){

    }

    void  disable_keys( std::vector<std::uint32_t> keys){

    }
//#endif

#else
    static void * hDevcieHandle = NULL;
    static void *hSessionHandle = NULL;


    bool get_private_key_access(uint32_t key_index){
        int ret;
        if( ret = SDF_GetPrivateKeyAccessRight(hSessionHandle, key_index, (unsigned char*)"11111111", 8)){
            std::cout<<"SDF_GetPrivateKeyAccessRight() error "<< std::hex << ret <<  std::endl;
            return false;
        }
        return true;
    }

    bool release_private_key_access(uint32_t key_index){
        if(SDF_ReleasePrivateKeyAccessRight(hSessionHandle, key_index)){
            std::cout<<"SDF_GetPrivateKeyAccessRight() error"<< std::endl;
            return false;
        }
        return true;
    }

    struct PrivateKeyAccessScope{
        PrivateKeyAccessScope(){}
        PrivateKeyAccessScope(uint32_t key_index){
            key_index_ = key_index;
            get_private_key_access(key_index_);
        }
        ~PrivateKeyAccessScope(){
            release_private_key_access(key_index_);
        }

        uint32_t key_index_;
    };

    bool init(){
        int ret = SDF_OpenDevice(&hDevcieHandle);
        if(ret){
            std::cout<< "OpenDevice() error." << std::hex << ret << std::endl;
            return false;
        }
        ret = SDF_OpenSession(hDevcieHandle, &hSessionHandle);
        if(ret){
            std::cout<< "OpenSession() error." << std::hex << ret << std::endl;
            return false;
        }

        return true;
    }

    void cleanup(){
        if( hSessionHandle){
            SDF_CloseSession(hSessionHandle);
            hSessionHandle = NULL;
        }
        if( hDevcieHandle){
            SDF_CloseDevice(hDevcieHandle);
            hDevcieHandle = NULL;
        }
    }

    void random_create(ByteArray &bytes,size_t size) {
        bytes.resize(size);
        SDF_GenerateRandom(hSessionHandle,(uint32_t)size,(unsigned char*)bytes.data());
    }

    struct KeyInfo{
        KeyID key_id;
        uint32_t key_index; // in card
        uint32_t algo_type; // ECC/RSA/AES
    };

    KeyInfo*  getKeyInfo(KeyID key_id,AlgorithmType algo,KeyInfo* info){
        info->key_index = 1;
        info->algo_type = (uint32_t) algo;
        return NULL ;
    }

    void pad_128(ByteArray& data){
        uint8_t pad = data.size() %16;
        uint8_t nopad[] = {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};
        if( pad == 0){
            std::copy_n( (char*)nopad,16,std::back_inserter(data));
        }else {
            std::fill_n(std::back_inserter(data), 16-pad, pad);
        }
    }

    bool unpad_128(ByteArray& data){
        if( data.size() % 16){
            return false;
        }
        uint8_t nopad[] = {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};

        if( memcmp( (const char*)&*(data.end()-16),nopad,16) == 0 ){
            data.erase( data.end() - 16,data.end());
        }else {
            uint8_t pad = data[data.size() - 1];
            data.erase(data.end() - 16 + pad, data.end());
        }
        return true;
    }

    void data_enc(KeyID key_id,AlgorithmType algo_type,const char * key, size_t key_size, const char* data,size_t  size,ByteArray &enc_data) {
        KeyInfo info;
        int ret ;
        getKeyInfo(key_id,algo_type, &info);
        PrivateKeyAccessScope _(info.key_index);

        if ( info.algo_type & SGD_SM2_ECC_BASE ){
            ECCCipher *ecccipher;
            enc_data.resize(size+ 164);
            ecccipher = (ECCCipher*)enc_data.data();


            ret = SDF_InternalEncrypt_ECC(hSessionHandle,info.key_index,(uint8_t*)data,(uint32_t)size,ecccipher);
            if(ret){
                enc_data.clear();
                std::cout<< "SDF_InternalEncrypt_ECC error: "<< std::hex << ret << std::endl;
            }
            enc_data.resize(ecccipher->L + 164);

        }else if( info.algo_type == SGD_RSA){
            uint8_t  _data[256];
            uint32_t _size;
            if( SDF_InternalPublicKeyOperation_RSA(hSessionHandle, info.key_index,(uint8_t*)data,(uint32_t)size,_data,&_size) ){
                std::cout<< " data_enc error." << std::endl;
            }else{
                enc_data.assign(_data,_data + _size);
            }
        }else{

            ByteArray plain(data,data+size);
            pad_128(plain);
            enc_data.resize( plain.size() );
            uint32_t enc_size;
            enc_size = enc_data.size();

            void * hKeyHandle;
            if(ret = SDF_SetKey(hSessionHandle,(unsigned char*)key,(uint32_t)key_size,&hKeyHandle)){
                std::cout <<" SDF_GenerateKeyWithKEK() error.." << std::hex<< ret << std::endl;
                return;
            }
            unsigned char iv[16];
            memset(iv,0,16);
            if( ret = SDF_Encrypt(hSessionHandle, hKeyHandle,info.algo_type,iv,(uint8_t*)plain.data(),(uint32_t)plain.size(), (uint8_t*) enc_data.data(), &enc_size)){
                std::cout<< " data_enc error." << std::hex << ret<< std::endl;
                return;
            }
           // enc_data.resize(enc_size);
        }
    }

    void data_dec2(KeyID key_id,std::uint64_t algo_type,const char * key, size_t key_size, const char* enc_data,size_t  size,ByteArray & data) {
        return;
    }
    void data_dec(KeyID key_id,AlgorithmType algo_type,const char * key, size_t key_size, const char* enc_data,size_t  size,ByteArray & data) {
        KeyInfo info;
        int ret ;
        uint32_t dec_size;

        getKeyInfo(key_id,algo_type, &info);
        PrivateKeyAccessScope _(info.key_index);

        if ( info.algo_type & SGD_SM2_ECC_BASE ){
            ECCCipher *ecccipher =(ECCCipher*) enc_data ;

            data.resize(size);
            dec_size = size;

            ret = SDF_InternalDecrypt_ECC(hSessionHandle,info.key_index,ecccipher,(unsigned char*) data.data(),&dec_size);
            if(ret){
                std::cout<< "SDF_InternalDecrypt_ECC error: "<< std::hex << ret << std::endl;
                data.clear();
                return;
            }
            data.resize(dec_size);


        }else if( info.algo_type == SGD_RSA){
            uint8_t  _data[256];

            if( SDF_InternalPrivateKeyOperation_RSA(hSessionHandle, info.key_index,(unsigned char*)enc_data,(uint32_t)size,_data,&dec_size) ){
                std::cout<< " data_enc error." << std::endl;
            }else{
                data.assign(_data,_data+dec_size);
            }
        }else{
            void * hKeyHandle;
            if(SDF_SetKey(hSessionHandle,(unsigned char*)key,(unsigned int) key_size,&hKeyHandle)){
                std::cout <<" SDF_GenerateKeyWithKEK() error.." << std::endl;
                return;
            }
            data.resize(size);
            unsigned char iv[16];
            memset(iv,0,16);
            if( SDF_Decrypt(hSessionHandle, hKeyHandle,info.algo_type,iv,(uint8_t*)enc_data,(uint32_t)size, (uint8_t*) data.data(), &dec_size)){
                std::cout<< " data_enc error." << std::endl;
                return;
            }
            data.resize(dec_size);
            unpad_128(data);
        }
    }

    void data_sign(KeyID key_id,AlgorithmType algo_type,const char * pri_key, size_t key_size, const char* data,size_t  size,ByteArray &enc_data) {
        int ret;
        KeyInfo info;
        uint32_t dec_size;
        getKeyInfo(key_id,algo_type, &info);
        PrivateKeyAccessScope _(info.key_index);

        enc_data.resize( sizeof(ECCSignature));
        ECCSignature* sig = (ECCSignature*) enc_data.data();
        if( ret = SDF_InternalSign_ECC(hSessionHandle,info.key_index,(unsigned char*)data,(unsigned int)size,sig) ){
            std::cout<< "SDF_InternalSign_ECC() error.." << std::hex << ret << std::endl;
        }
    }

    bool data_verify(KeyID key_id,AlgorithmType algo_type,const char * pub_key, size_t key_size,const char* hash_data,size_t hash_size, const char* sign_data,size_t  sign_size) {
        int ret;
        KeyInfo info;
        uint32_t dec_size;
        getKeyInfo(key_id,algo_type, &info);
        PrivateKeyAccessScope _(info.key_index);

        ECCSignature* sig = (ECCSignature*)sign_data;
        if( ret = SDF_InternalVerify_ECC(hSessionHandle,info.key_index,(unsigned char*)hash_data,(unsigned int)hash_size, sig) ){
            std::cout << "SDF_InternalVerify_ECC() error.." << std::hex << ret << std::endl;
            return false;
        }
        return true;
    }

    void data_hash(AlgorithmType algo_type, const char* data,size_t  size,ByteArray &hash) {
        // don't support SGD_SM3 now !
//        r = SDF_HashInit(hSessionHandle, SGD_SM3, &eccpubkey, "1234567812345678", 16);
//        SGD_SM3
        int ret;
        if(ret = SDF_HashInit(hSessionHandle, algo_type, NULL, NULL, 0) ){
            std::cout<< "SDF_HashInit() error.." << std::hex << ret<< std::endl;
            return;
        }
        if( ret = SDF_HashUpdate(hSessionHandle, (unsigned char*)data, sizeof(data)) ){
            std::cout<< "SDF_HashInit() error.." << std::hex << ret << std::endl;
            return;
        }
        hash.resize(256);
        uint32_t hash_size;
        if( ret = SDF_HashFinal(hSessionHandle,(unsigned char*) hash.data(), &hash_size)){
            std::cout<< "SDF_HashFinal() error.." << std::hex << ret <<std::endl;
            hash.clear();
            return;
        }
        hash.resize(hash_size);
    }

    KeyID get_current_keyid(){
        return  0;
    }

    void  set_current_keyid(KeyID){

    }

    void  import_keys( std::vector<SecKeyBody>& keys){

    }

    void  destroy_keys(){
    }

    void  enable_keys(std::vector<std::uint32_t> keys){

    }

    void  disable_keys( std::vector<std::uint32_t> keys){

    }


 // SDF_MAX_KEY_INDEX 0x0f / 0-15

    bool create_keypair(uint32_t index, KeyCreateType type,uint32_t  bit_size,ByteArray & data){
        unsigned int sign_flag = type & 0x01;
        int ret ;
        PrivateKeyAccessScope access(index);
        if( (type & (1<<8)) == 0){ //ECC

            data.resize( sizeof(ECCrefPublicKey));
            ECCrefPublicKey *pucPublicKey = (ECCrefPublicKey *) data.data();
            if( ret = SDF_CreateKeyPair_ECC(hSessionHandle,sign_flag,(unsigned int)index,pucPublicKey)){
                std::cout<< "SDF_CreateKeyPair_ECC() error.." << std::hex<<  ret << std::endl;
                data.clear();
                return false;
            }
        }else{
            data.resize( sizeof( RSArefPublicKey ));
            RSArefPublicKey * publickey = (RSArefPublicKey*) data.data();
            if( ret = SDF_CreateKeyPair_RSA(hSessionHandle,sign_flag,(unsigned int)index,bit_size,publickey)){
                std::cout<< "SDF_CreateKeyPair_ECC() error.." << std::hex << ret << std::endl;
                data.clear();
                return false;
            }
        }
        return true;
    }

#endif


}