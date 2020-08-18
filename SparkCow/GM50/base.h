//
// Created by scott on 7/29/20.
//

#ifndef MONKEY_GM50_BASE_H
#define MONKEY_GM50_BASE_H

#include <cstdint>
#include <numeric>
#include <vector>
#include <memory>


enum class Version{
    Version = 1
};

enum class OperationType{
    ChannelCreateRequest = 0xA1,    // agent -> mgr
    ChannelCreateResponse = 0xA2 ,   // mgr -> agent
    ChannelMessageSend = 0xA3,      // agent <-> mgr
    ChannelReCreateRequest = 0xA4   // mgr -> agent
};

enum  class AppID{
    DeviceManagement = 0x00, // GM50
    SecKeyManagement = 0xC0, // GM51
    RemoteMonitor = 0xC1,  //GM53
    ParamConfigure = 0xC2,
    RemoteMaintenance = 0xC3,
    ComplianceTest = 0xC4,
    VpnManagement = 0xC5,
    UserAppExt = 0xF0,  //
    UserAppJsonData = UserAppExt
};


enum class SafeModeField{
    Reply = 1<<2,
    Encrypt = 1<< 1,
    Sign = 1 << 0,
    All = Reply | Encrypt | Sign
};


//typedef char DeviceID[32] ;

typedef std::vector< char >  ByteArray;
typedef std::vector< char >  DeviceID;


#define TYPE_DECL(cls) typedef std::shared_ptr< ##cls## > Ptr;
//    typedef std::vector< Ptr > Array;\


enum class CommandType{
    Unknown = 0,
    Get = 0xB0,
    GetNext = 0xB1,
    Response = 0xB2,
    Set = 0xB3,
    GetBulk = 0xB4,
    Inform = 0xB5,
    Trap = 0xB6,

    RemoteMonitorRequest = 0xC3 , // platform -> device
    RemoteMonitorResponse = 0xC4, // device -> platform

    ComplianceTestRequest = 0x93,   // platform -> device
    ComplianceTestResponse = 0x94,   // device -> platform

    // platform -> device
    SecKeyManagementNegotiateRequest = 0xB0,
    SecKeyManagementNegotiateResponse = 0xB1,
    SecKeyManagementDistributeRequest = 0xB2,
    SecKeyManagementDistributeResponse = 0xB3,
    SecKeyManagementDestroyRequest = 0xB4,
    SecKeyManagementDestroyResponse = 0xB5,
    SecKeyManagementEnableRequest = 0xB6,
    SecKeyManagementEnableResponse = 0xB7,

    SecKeyManagementApplyRequest = 0xB8, // Key Apply Request
    SecKeyManagemenApplyResponse = 0xB9, // Key Apply Act

    // user command list
    UserJsonData = 0x01,

};


enum class AlgorithmType{
    Sym = 0,
    Asym = 1,
    Hash = 2
};

enum class EncryptMode{
    ECB = 0,
    CBC = 1,
    CFB = 2,
    OFB = 3,
    RSA = 0 ,
    SM2 = 1
};

enum class EncryptType{
    ENCRYPT = 0 ,
    DECRYPT = 1,
    SIGN = 2,
    VERSIGN = 3,
    KEYXCHG = 4
};


#define KMR_OK 0
#define KMR_BASE  0x0E000000
#define KMR_UNKNOWN_ERR KMR_BASE + 1


#define DECL_SINGLETON(cls) \
public:\
\
static std::shared_ptr< cls >& instance(){\
    static std::shared_ptr< cls > handle ;\
    if(!handle.get()){\
        handle = std::make_shared< cls >() ;\
    }\
    return handle;\
}\

#endif //MONKEY_BASE_H
