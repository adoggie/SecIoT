//
// Created by scott on 7/29/20.
//

#ifndef MONKEY_DATATYPE_H
#define MONKEY_DATATYPE_H

#include <cstdint>
#include <string>
#include <vector>

typedef std::uint8_t  SDM_BYTE;
typedef std::int16_t SDM_INT16;
typedef std::uint16_t SDM_UINT16;

typedef std::int32_t SDM_INT32;
typedef std::int64_t SDM_INT64;

typedef std::uint32_t SDM_UINT32;
typedef std::uint64_t SDM_UINT64;

typedef std::string  SDM_OCTET_STRING;

typedef SDM_UINT32  SDM_COUNTER32;
typedef SDM_UINT64  SDM_COUNTER64 ;
typedef SDM_UINT64  SDM_AID ;
typedef SDM_UINT32  SDM_TimeTicks;

typedef SDM_OCTET_STRING SDM_DisplayString;
typedef SDM_OCTET_STRING SDM_MacAddress;
typedef SDM_UINT32  SDM_TestAndIncr;
typedef SDM_OCTET_STRING    SDM_DateAndTimeS;

typedef struct{
    std::uint16_t  year;  // 1 - 9999
    std::uint8_t   month; // 1 - 12
    std::uint8_t   day;  // 1- 31
    std::uint8_t   hour; // 0 - 23
    std::uint8_t   min;  // 0- 59
    std::uint8_t   sec;  // 0- 60
}SDM_DateAndTime;

typedef std::string SDM_PubKey_Info;
typedef std::string SDM_Sign_Info;
typedef std::string SDM_Cert_Application;
typedef std::string SDM_Cert ;

enum  class SDM_TrueValue{
    True = 1,
    False = 0
};

enum class SDM_RowStatus{
    Active = 1,
    NotInService = 2,
    NotReady = 3 ,
    CreateAndWait = 4 ,
    CreateAndGo = 5,
    Destroy = 6
};

enum class SDM_StorageType{
    Other = 1,
    Volatile = 2 ,
    NonVolatile = 3 ,
    Permanent = 4,
    ReadOnly = 5
};



#define SDM_NULL  0



#endif //MONKEY_DATATYPE_H
