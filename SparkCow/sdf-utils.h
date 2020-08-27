//
// Created by scott2 on 8/6/20.
//

#ifndef MONKEY_SDF_UTILS_H
#define MONKEY_SDF_UTILS_H

#include <string>
#include <map>
#include <vector>

#include "GM50/apps/SecKeyManagement.h"
#include "SDF/sdf.h"

namespace sdf{
    typedef std::vector<char> ByteArray;
    typedef ByteArray PublicKey;
    typedef ByteArray PrivateKey;

    typedef std::uint64_t KeyID;
    typedef std::uint64_t AlgorithmType;

    void random_create(ByteArray& byte,size_t size);
//    void sm2_enc(PublicKey &pub_key, const char* data, size_t size,ByteArray &encdata) ;
//    void sm2_dec(PrivateKey &prv_key, const char* crypt_data,size_t  size,ByteArray &plain_data) ;
//    void sm2_sign(PrivateKey & prv_key , const char* data , size_t  size,ByteArray& signature);
//    bool sm2_verify(PublicKey & prv, const char* data, size_t size,  const char*  sign , size_t sign_len);
//    void sm4_enc(const char* data , size_t data_size, const char* key,size_t key_size, ByteArray& enc_data);
//    void sm4_dec(const char* data, size_t data_size, const char * key ,size_t key_size,ByteArray& dec_data);

    void data_enc(KeyID key_id,AlgorithmType algo_type,const char * key, size_t key_size, const char* data,size_t  size,ByteArray &enc_data) ;
    void data_dec(KeyID key_id,AlgorithmType algo_type,const char * key, size_t key_size, const char* enc_data,size_t  size,ByteArray & data) ;
    void data_sign(KeyID key_id,AlgorithmType algo_type,const char * pri_key, size_t key_size, const char* data,size_t  size,ByteArray &enc_data) ;
    bool data_verify(KeyID key_id,AlgorithmType algo_type,const char * pub_key, size_t key_size,const char* hash_data,size_t hash_size, const char* sign_data,size_t  sign_size) ;

    void data_hash(AlgorithmType algo_type, const char* data,size_t  size,ByteArray &hash) ;

    void data_dec2(KeyID key_id,std::uint64_t algo_type,const char * key, size_t key_size, const char* enc_data,size_t  size,ByteArray & data) ;


    KeyID get_current_keyid();
    void  set_current_keyid(KeyID);
    void  import_keys( std::vector<SecKeyBody>& keys);
    void  destroy_keys();
    void  enable_keys(std::vector<std::uint32_t> keys);
    void  disable_keys( std::vector<std::uint32_t> keys);

    bool init();
    void cleanup();

    // key for sign  1
    // key for cipher 0
    enum KeyCreateType{
        KeyCreateForSignECC =0x0001,
        KeyCreateForCipherECC = 0x0000,
        KeyCreateForSignRSA = 0x0101,
        KeyCreateForCipherRSA = 0x0100
    };

    bool create_keypair(uint32_t key_index, KeyCreateType type,uint32_t  bit_size,ByteArray & data);

}

#endif //MONKEY_SDF_UTILS_H
