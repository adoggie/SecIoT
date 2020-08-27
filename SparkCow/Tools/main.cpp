//
// Created by scott on 8/20/20.
//

#include "../sdf-utils.h"
#include <boost/beast/core/detail/base64.hpp>
// index : 1 - 16

int main(int argc, char ** argv){
    sdf::init();
    ByteArray bytes;
//    sdf::random_create(bytes,16);
//    sdf::create_keypair(1,sdf::KeyCreateForSignECC,0,bytes);
//    sdf::create_keypair(1,sdf::KeyCreateForCipherECC,0,bytes);
//    sdf::create_keypair(1,sdf::KeyCreateForSignRSA,1024,bytes);

    std::string ecc_sign_key = "AAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAPKYcsXJmtlPOB75ivbJz0LAg7pIMQkpQsHYiMU1OIt8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAP9sivXzC7t4cFRx2mBw/QUbVTyYcFvHDRsDrn+azKKG";
    std::string ecc_encrypt_key="AAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAP5pnvH3XnhBnD6f3qnBLeYz2eWUTvVtsvALTuux9StkAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAK7wyff/QF5YqaEvcNci4bwPixwO2rSAOSIw1cXMtQzi";

    bytes.resize(boost::beast::detail::base64::decoded_size(ecc_encrypt_key.size()));
//    auto const result = boost::beast::detail::base64::decode(
//            &bytes[0], ecc_encrypt_key.data(), ecc_encrypt_key.size());

    ByteArray data;
    std::string plain = "shanghai china no god no people bibleqwkejqwlkjelqwkjqwjeklqwjeqwekqlwke";

    sdf::data_enc(1,SGD_SM2_1,bytes.data(),bytes.size(),plain.data(),36,data);
    ByteArray decode_data;
    sdf::data_dec(1,SGD_SM2_1,NULL,0,data.data(),data.size(), decode_data);



    {
        std::string key ="abc";
        sdf::data_enc(1, SGD_SM1_CFB, key.data(), key.size(), plain.data(), 33, data);
        ByteArray dec_data;
        sdf::data_dec(1, SGD_SM1_CFB, key.data(), key.size(), data.data(), data.size(), dec_data);
        std::cout<<std::endl;
        std::for_each( dec_data.begin(),dec_data.end(),[](char& c){ std::cout << std::hex << c ;});
        std::cout<<std::endl;
    }

    { // sign
        data.clear();
        sdf::data_sign(1,SGD_SM2_1 ,NULL,0,plain.data(),32, data);
    }
    { // verify
        bytes.resize(boost::beast::detail::base64::decoded_size(ecc_sign_key.size()));
        auto const result = boost::beast::detail::base64::decode(
                &bytes[0], ecc_sign_key.data(), ecc_sign_key.size());

        bool succ = sdf::data_verify(1,SGD_SM2_1,bytes.data(),bytes.size(),plain.data(),plain.size(), data.data(),data.size());

        std::cout<<"verify :" << succ << std::endl;

    }
    {

        sdf::data_hash(SGD_SM3,plain.data(),plain.size(),data);

    }

    sdf::cleanup();
    return 0;
}