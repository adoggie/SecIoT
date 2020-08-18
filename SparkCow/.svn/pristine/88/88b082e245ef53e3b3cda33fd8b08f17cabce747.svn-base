//
// Created by scott on 2020-08-17.
//

#ifndef SMARTBOX_SPARKCOW_H
#define SMARTBOX_SPARKCOW_H

#include <stdint.h>


typedef struct{
    uint8_t * data;
    uint32_t len;
    void * any;
}sparkcow_bytearray_t;

typedef struct{
    sparkcow_bytearray_t *local_device_id;
    sparkcow_bytearray_t *remote_device_id;
    sparkcow_bytearray_t *local_public_key;
    sparkcow_bytearray_t *local_private_key;
    sparkcow_bytearray_t *remote_public_key;

}sdf_endpoint_cfgs_t;


typedef enum  {
    avt_String = 0,
    avt_Int64 = 0x08,
    avt_Int32 = 0x04 ,
    avt_Int16 = 0x02,
    avt_Int8 =  0x01,

    avt_UInt64 = 0x08,
    avt_UInt32 = 0x04 ,
    avt_UInt16 = 0x02,
    avt_UInt8 =  0x01,

    avt_SDM_TimeTicks = 0x04,
    avt_SDM_Cert = 0x00,
    avt_SDM_RowStatus = 0x01,
    avt_SDM_AID = 0x08,

}sparkcow_attr_value_type_t;

#define sparkcow_attr_typeid_def(a,b) ( a<<8 | b)


typedef struct{
    int type;

    union {
        uint64_t u64;
        uint32_t u32;
        int64_t  i64;
        int32_t  i32;
        uint16_t u16;
        int16_t  i16;
//        char*    data;
        sparkcow_bytearray_t  str;
    } val;
    uint32_t  size;
}sparkcow_attr_value_t;

sparkcow_attr_value_t * sparkcow_new_attr_value();
void sparkcow_new_attr_value(sparkcow_attr_value_t *);

enum sparkcow_attr_type_t{
    attr_basic_vendor = sparkcow_attr_typeid_def(1,avt_String),  //hi8: 厂商 ,lo8: 值类型( 0: string)
    attr_basic_model = sparkcow_attr_typeid_def(1,avt_String),  //hi8: 型号 ,lo8: 值类型( 0: string)
    attr_basic_sn = sparkcow_attr_typeid_def(1,avt_String),  //hi8: 设备序列号 ,lo8: 值类型( 0: string)
    attr_basic_sys_ver = sparkcow_attr_typeid_def(1,avt_String),  //hi8: 系统版本号 ,lo8: 值类型( 0: string)
    attr_basic_mgm_ver = sparkcow_attr_typeid_def(1,avt_String),  //hi8: 管理协议版本号 ,lo8: 值类型( 0: string)
    attr_basic_dev_desc = sparkcow_attr_typeid_def(1,avt_String),  //hi8: 设备描述 ,lo8: 值类型( 0: string)
    attr_basic_runtime_ticks = sparkcow_attr_typeid_def(1,avt_SDM_TimeTicks),  //hi8: 已运行时间 ,lo8: 值类型( 0: string)
    attr_basic_contact = sparkcow_attr_typeid_def(1,avt_String),  //hi8: 联系方式 ,lo8: 值类型( 0: string)
    attr_basic_dev_position = sparkcow_attr_typeid_def(1,avt_String),  //hi8: 设备物理位置 ,lo8: 值类型( 0: string)
    attr_basic_asymm_algorithm = sparkcow_attr_typeid_def(1,avt_UInt64),  //hi8: 非对称算法 ,lo8: 值类型( 0: string)
    attr_basic_symm_algorithm = sparkcow_attr_typeid_def(1,avt_UInt32),  //hi8: 对称算法 ,lo8: 值类型( 0: string)
    attr_basic_hash_algorithm = sparkcow_attr_typeid_def(1,avt_UInt32),  //hi8: 哈希算法 ,lo8: 值类型( 0: string)

    attr_group_id = sparkcow_attr_typeid_def(0x21,avt_UInt64),  //hi8: 组对象id
    attr_group_desc = sparkcow_attr_typeid_def(0x21,avt_String),  //hi8: 组对象描述
    attr_group_update_time = sparkcow_attr_typeid_def(0x21,avt_SDM_TimeTicks),  //hi8: 上次该行赋值时间
    attr_group_control_status = sparkcow_attr_typeid_def(0x21,avt_UInt8),  //hi8: 控制状态

    attr_device_id = sparkcow_attr_typeid_def(0x21,avt_String),  //hi8: 控制状态
    attr_cert_application = sparkcow_attr_typeid_def(0x21,avt_SDM_Cert),  //hi8: 证书申请
    attr_center_cert = sparkcow_attr_typeid_def(0x21,avt_SDM_Cert),  //hi8: 总中心证书
    attr_parent_cert = sparkcow_attr_typeid_def(0x21,avt_SDM_Cert),  //hi8: 父中心证书
    attr_mgt_cert = sparkcow_attr_typeid_def(0x21,avt_SDM_Cert),  //hi8: 父中心证书

    attr_if_num = sparkcow_attr_typeid_def(0x41,avt_UInt32),  // 接口数量
    attr_if_name = sparkcow_attr_typeid_def(0x41,avt_String),  // 接口名称
    attr_if_desc = sparkcow_attr_typeid_def(0x41,avt_String),  // 接口描述
    attr_if_type = sparkcow_attr_typeid_def(0x41,avt_UInt32),  // 接口类型
    attr_if_addr = sparkcow_attr_typeid_def(0x41,avt_String),  // 接口地址
    attr_if_mtu = sparkcow_attr_typeid_def(0x41,avt_UInt32),  // 最大包长
    attr_if_speed = sparkcow_attr_typeid_def(0x41,avt_UInt64),  // 接口速率
    attr_if_in_packet = sparkcow_attr_typeid_def(0x41,avt_UInt64),  // 收到包总数
    attr_if_out_packet = sparkcow_attr_typeid_def(0x41,avt_UInt64),  // 发送包总数
    attr_if_err_packet = sparkcow_attr_typeid_def(0x41,avt_UInt64),  // 错误包总数
    attr_if_updown_trap_enable = sparkcow_attr_typeid_def(0x41,avt_UInt32),  // 是否允许发送trap包
    attr_if_change_time = sparkcow_attr_typeid_def(0x41,avt_SDM_TimeTicks),  // 是否允许发送trap包
    attr_if_control_status = sparkcow_attr_typeid_def(0x41,avt_SDM_RowStatus),  // 添加，删除状态变量

    attr_mg_in_packet = sparkcow_attr_typeid_def(0x41,avt_UInt32),  // 收到管理包数
    attr_mg_out_packet = sparkcow_attr_typeid_def(0x41,avt_UInt32),  // 发送管理包数
    attr_mg_err_packet = sparkcow_attr_typeid_def(0x41,avt_UInt32),  // 错误管理包数
    attr_mg_alert_packet = sparkcow_attr_typeid_def(0x41,avt_UInt32),  // 告警管理包数

};

typedef struct{
    sparkcow_attr_value_t* dev_mgt_get_attr(int type,int index);
    void dev_mgt_set_attr(int type,int index,sparkcow_attr_type_t* value);
    void attr_free(sparkcow_attr_value_t* attr);
    sparkcow_attr_value_t* attr_new();

}sparkcow_management_defs_t;

typedef struct {
    sparkcow_management_defs_t  management;
    void (*open)(char * tcp_addr, int tcp_port);
    void (*close)();
    void (*send)(sparkcow_bytearray_t* data);
    void (*data_free)(sparkcow_bytearray_t * data);
    sparkcow_bytearray_t* data_new(const char* data,uint32_t size);

    void (*on_connected)();
    void (*on_disconnected)();
    void (*on_error)(int error);
    void (*on_data)(sparkcow_bytearray_t* data);
    sdf_endpoint_cfgs_t* (*on_get_endpoint_cfgs)();

    void * user;
}sparkcow_context_t;


int sparkcow_init(sparkcow_context_t* ctx);
void sparkcow_cleanup(sparkcow_context_t* ctx);




#endif //SMARTBOX_SPARKCOW_H
