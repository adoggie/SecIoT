//
// Created by scott2 on 8/6/20.
//

#include "sdf-utils.h"
//#include <sdf_random.h>
//#include <sdf_random.h>
//#include "sdf_sm3.h"
//#include "sdf_sm4.h"
//#include "sdf_sm2_private_key.h"
//#include "sdf_sm2_public_key.h"
//#include "sdf_interior.h"

#include <algorithm>
#include <iterator>

namespace sdf {

    void random_create(ByteArray &bytes,size_t size) {
        bytes.resize(size);
        std::fill_n(bytes.begin(),size,0xFF);
//        if (bytes.size() == 0) {
//            bytes.resize(32);
//
//        }
//
//        rng_t *rng_ = random_rng_create(RNG_STRONG);
//        rng_->get_bytes(rng_, bytes.size(), (uint8_t *) bytes.data());
//        rng_->destroy(rng_);
//        return true;
    }


//    void sm2_enc(PublicKey &pub_key, const char* data, size_t size,ByteArray &encdata) {
//
//    }
//
//    void sm2_dec(PrivateKey &prv_key, const char* crypt_data,size_t  size,ByteArray &plain_data) {
//
//    }
//
//    void sm2_sign(PrivateKey & prv_key , const char* data , size_t  size,ByteArray& signature){
//
//    }
//
//    bool sm2_verify(PublicKey & prv, const char* data, size_t size,  const char*  sign , size_t sign_len){
//        return false;
//    }
//
//    void sm4_enc(const char* data , size_t data_size, const char* key,size_t key_size, ByteArray& enc_data){
//
//    }
//
//    void sm4_dec(const char* data, size_t data_size, const char * key ,size_t key_size,ByteArray& dec_data){
//
//    }


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


}