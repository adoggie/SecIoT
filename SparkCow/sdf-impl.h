//
// Created by scott on 2020-08-13.
//

#ifndef SMARTBOX_SDF_IMPL_H
#define SMARTBOX_SDF_IMPL_H

// v0.2 2020.8.13

#include <numeric>

#define USER_FREE // 用户需要使用 free 释放 内存

typedef struct{
    uint8_t * data;
    size_t len;
    void * any;
}sdf_bytearray_t,sdf_result_t;

// gm51 /7.2.2
typedef struct {
    uint64_t     key_id;
    uint32_t     key_type;
    uint32_t     key_len;
    sdf_bytearray_t         key_set;
    uint32_t     key_check_val_alg;
    uint8_t      key_check_val_len;
    sdf_bytearray_t         key_check_val;
    uint32_t     key_sys_id;
    uint32_t     key_device_id;

}sdf_seckeybody_t;

typedef uint64_t sdf_keyid_t;
typedef uint64_t sdf_algorithm_type_t;

// 创建随机数
// result : .data 接收地址, .len 随机数长度 , 用户分配内存
void sdf_random_create(sdf_bytearray_t* result);
void sdf_data_free(sdf_bytearray_t * data);

// 加密
// key_id : 卡内密钥编号 ,
// algo_type: 加密算法 GM06
// key: 密钥信息 , 对称和非对称密钥(pub/private) , NULL表示采用卡内密钥
//   .data  密钥地址
//   .len  密钥长度
// data :  明文信息   .data 数据地址  ， .len 数据长度
// reuslt: 加密数据结果, 底部分配内存，外部通过 sdf_data_free 释放
//   .data : 加密数据地址
//   .len : 加密数据长度

void sdf_data_enc(sdf_keyid_t key_id,sdf_algorithm_type_t algo_type,sdf_bytearray_t* key, sdf_bytearray_t* data ,sdf_result_t * result USER_FREE) ;

// 数据解密
// key_id : 卡内密钥编号 ,
// algo_type: 加密算法 GM06
// key: 密钥信息 , 对称和非对称密钥(pub/private) , NULL表示采用卡内密钥
//   .data  密钥地址
//   .len  密钥长度
// cipher_text :  密文信息   .data 数据地址  ， .len 数据长度
// reuslt: 解密数据结果, 底部分配内存，外部通过 sdf_data_free 释放
//   .data : 解密数据地址
//   .len : 解密数据长度

void sdf_data_dec(sdf_keyid_t key_id,sdf_algorithm_type_t algo_type,sdf_bytearray_t* key, sdf_bytearray_t* cipher_text,sdf_result_t * result USER_FREE) ;


// 数据签名
// key_id : 卡内密钥编号 ,
// algo_type: 加密算法 GM06
// pri_key: 私钥信息 , NULL表示采用卡内密钥
//   .data  密钥地址
//   .len  密钥长度
// data :  明文信息   .data 数据地址  ， .len 数据长度
// reuslt: 签名数据结果, 底部分配内存，外部通过 sdf_data_free 释放
//   .data : 数据地址
//   .len : 数据长度

void sdf_data_sign(sdf_keyid_t key_id,sdf_algorithm_type_t algo_type,sdf_bytearray_t* pri_key, sdf_bytearray_t* data ,sdf_result_t *result USER_FREE ) ;

// 数据验签
// key_id : 卡内密钥编号 ,
// algo_type: 加密算法 GM06
// pub_key: 公钥信息 , NULL表示采用卡内密钥
//   .data  密钥地址
//   .len  密钥长度
// data :  明文信息   .data 数据地址  ， .len 数据长度
// sign: 签名数据,
//   .data : 数据地址
//   .len : 数据长度

// @return: true / false
bool sdf_data_verify(sdf_keyid_t key_id,sdf_algorithm_type_t algo_type,sdf_bytearray_t* pub_key,sdf_bytearray_t* data, sdf_bytearray_t* sign) ;


// 数据散列计算
// key_id : 卡内密钥编号 ,
// algo_type: 加密算法 GM06
// data :  明文信息   .data 数据地址  ， .len 数据长度
// hash: 散列数据, 底部分配内存，外部通过 sdf_data_free 释放
//   .data : 数据地址
//   .len : 数据长度
void sdf_data_hash(sdf_keyid_t key_id,sdf_algorithm_type_t algo_type, sdf_bytearray_t* data ,sdf_result_t *hash USER_FREE ) ;

// 查询当前卡内默认key编号
sdf_keyid_t sdf_get_current_keyid();

// 设置当前卡内默认key编号
void  sdf_set_current_keyid(sdf_keyid_t key_id);

// 导入多个密钥到设备
void  sdf_import_keys( sdf_seckeybody_t* keys , size_t size);

// 销毁所有密钥
void  sdf_destroy_keys();

// 启用多个key
void  sdf_enable_keys(sdf_keyid_t* keys ,size_t size);

// 禁用多个key
void  sdf_disable_keys( sdf_keyid_t* keys ,size_t size );

#endif //SMARTBOX_SDF_IMPL_H
