//
// Created by scott on 8/4/20.
//

#ifndef MONKEY_REMOTEMONITOR_H
#define MONKEY_REMOTEMONITOR_H


#include "../command.h"
#include "../attribute.h"

typedef std::uint64_t VID;

#define DECL_VID(group,app_id,attr,row,sub) ( ( std::uint8_t(group) & 0xff) | ( std::uint64_t(app_id) << 8) | ( std::uint64_t(attr) << 24) | ( std::uint64_t(row) << 32)  | ( std::uint64_t(sub) << 56)  )

#define VID_KeyStatus           DECL_VID(5,0,2,0,0)
#define VID_KeyUpdateStatus     DECL_VID(5,0,3,0,0)
#define VID_KeyTunnelStatus     DECL_VID(5,0,4,0,0)
#define VID_KeyLostMonitor      DECL_VID(5,0,5,0,0)
#define VID_KeyIPChanged        DECL_VID(5,0,8,0,0)
#define VID_RandGeneratorStatus DECL_VID(5,0,9,0,0)
#define VID_KeyProgramCheck     DECL_VID(5,0,0x0a,0,0)
#define VID_LogNum              DECL_VID(6,0,1,0,0)
#define VID_LogTable            DECL_VID(6,0,2,0,0)


/// GM53/6.1

struct CommandRemoteMonitorRequest:    Command{
    typedef std::shared_ptr<CommandRemoteMonitorRequest> Ptr;

    DeviceID    device_id;
    ByteArray   params; // 14 bytes
    VID         vid;
public :
    CommandRemoteMonitorRequest():Command(CommandType::RemoteMonitorRequest, AppID::RemoteMonitor){
        params.resize(14);
    }

    void marshall( ByteStream& stream);
    static CommandRemoteMonitorRequest::Ptr parse(ByteStream& stream);
};

struct CommandRemoteMonitorResponse:    CommandRemoteMonitorRequest{
    typedef std::shared_ptr<CommandRemoteMonitorResponse> Ptr;
public :
    CommandRemoteMonitorResponse(){ //:Command(CommandType::RemoteMonitorResponse, AppID::RemoteMonitor){
        app_id = AppID::RemoteMonitor;
        cmd_type = CommandType::RemoteMonitorResponse;
    }

//    void marshall( ByteStream& stream);
    static CommandRemoteMonitorResponse::Ptr parse(ByteStream& stream);
};




class RemoteMonitor {

};


#endif //MONKEY_REMOTEMONITOR_H
