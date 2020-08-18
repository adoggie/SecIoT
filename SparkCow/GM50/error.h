//
// Created by scott on 7/30/20.
//

#ifndef MONKEY_ERROR_H
#define MONKEY_ERROR_H

#include <string>

struct Consts{
    enum ErrorCode{
        SUCC = 0,
        SENDFAILED =1,
        DATADIRTY= 3,
        TIMEOUT = 2,
        INTERFACE_NOTFOUND = 4,
        UNSERIALIZE_FAILED = 5,
        REMOTEMETHOD_EXCEPTION = 6,
        DATA_INSUFFICIENT = 7,
        REMOTE_EXCEPTION = 8,

        CONNECT_UNREACHABLE = 101,
        CONNECT_FAILED  = 102,
        CONNECT_REJECT = 103,
        CONNECTION_LOST = 104	//连接丢失
    };

    struct error_info_t{
        int code;
        const char* str;
    };

    static error_info_t* get_error(int ec){
        static error_info_t _errors[]={
                {SENDFAILED,"SENDFAILED"},
                {DATADIRTY,"DATADIRTY"},
                {TIMEOUT,"TIMEOUT"},
                {INTERFACE_NOTFOUND,"INTERFACE_NOTFOUND"},
                {UNSERIALIZE_FAILED,"UNSERIALIZE_FAILED"},
                {REMOTEMETHOD_EXCEPTION,"REMOTEMETHOD_EXCEPTION"},
                {DATA_INSUFFICIENT,"DATA_INSUFFICIENT"},
                {REMOTE_EXCEPTION,"REMOTE_EXCEPTION"},

                {CONNECT_UNREACHABLE,"CONNECT_UNREACHABLE"},
                {CONNECT_FAILED,"CONNECT_FAILED"},
                {CONNECT_REJECT,"CONNECT_REJECT"},
                {CONNECTION_LOST,"CONNECTION_LOST"},
                {-1,NULL},
        };

        error_info_t * err = NULL;
        size_t n= 0;
        for(;n< sizeof(_errors)/sizeof(error_info_t);n++){
            if( _errors[n].str == NULL){
                break;
            }
            if( _errors[n].code == ec){
                err = &_errors[n];
                break;
            }
        }
        return err;
    }



    enum CompressType{
        COMPRESS_NONE = 0, //	#压缩方式ß
        COMPRESS_ZLIB = 1,
        COMPRESS_BZIP2 = 2,
    };

    enum EncryptType{
        ENCRYPT_NONE = 0,  // #加密方式
        ENCRYPT_MD5  = 1,
        ENCRYPT_DES  = 2,
    };

    enum MsgType{
        MSGTYPE_RPC = 1,
        MSGTYPE_NORPC = 2
    };

};


struct Exception{
    Exception(Consts::ErrorCode code_,int subcode_=0,const std::string & msg_="" ){
        errcode = code_;
        subcode = subcode_;
        msg = msg_;
    }

    std::string what() const{
        Consts::error_info_t * err = NULL;
        err = Consts::get_error(errcode);
        if(err){
            return err->str;
        }
        return "unRegistered error";
    }

    int errcode;
    int subcode;
    std::string msg;
};



#endif //MONKEY_ERROR_H
