//
// Created by scott on 8/4/20.
//

#include "RemoteMonitorImpl.h"
#include "../GM50/apps/RemoteMonitor.h"
#include "../ManagedAgent.h"
#include <algorithm>
#include <iterator>

void RemoteMonitorImpl::onCommand(const Command::Ptr& command){

    {
        CommandRemoteMonitorRequest::Ptr  cp = std::dynamic_pointer_cast<CommandRemoteMonitorRequest>(command);
        if( cp){
            onCommandRemoteMonitorRequest(cp);
            return;
        }
    }
    {
        CommandRemoteMonitorResponse::Ptr  cp = std::dynamic_pointer_cast<CommandRemoteMonitorResponse>(command);
        if(cp){
            onCommandRemoteMonitorResponse(cp);
            return;
        }
    }
}

// gm53/6.1.3
void RemoteMonitorImpl::onCommandRemoteMonitorRequest(const CommandRemoteMonitorRequest::Ptr& command){
    auto cp = std::make_shared<CommandRemoteMonitorResponse>();
    cp->vid = command->vid;
    ByteStream data;
    auto & settings = ManagedAgent::instance()->getSettings();
    if( command->vid == VID_KeyStatus){
        data.writeByte((uint8_t) settings.vid_monitor.KeyStatus );
    }
    if( command->vid == VID_KeyUpdateStatus){
        data.writeByte((uint8_t) settings.vid_monitor.KeyUpdateStatus );
    }
    if( command->vid == VID_KeyTunnelStatus){
        data.writeByte((uint8_t) settings.vid_monitor.KeyTunnelStatus );
    }
    if( command->vid == VID_KeyLostMonitor){
        data.writeByte((uint8_t) settings.vid_monitor.KeyLostMonitor );
    }
    if( command->vid == VID_KeyIPChanged){
        data.writeByte((uint8_t) settings.vid_monitor.KeyIPChanged );
    }
    if( command->vid == VID_RandGeneratorStatus){
        data.writeByte((uint8_t) settings.vid_monitor.RandGeneratorStatus );
    }

    if( command->vid == VID_KeyProgramCheck){
        data.writeByte((uint8_t) settings.vid_monitor.KeyProgramCheck );
    }

    if( command->vid == VID_LogNum){
        data.writeByte((uint32_t) settings.vid_monitor.LogNum );
    }
    if( command->vid == VID_LogTable){
        ByteArray any(13);
        data.writeBytes(any.data(),any.size());
    }
    std::copy(data.data(),data.data()+data.size(), std::back_inserter(cp->params));
}

void RemoteMonitorImpl::onCommandRemoteMonitorResponse(const CommandRemoteMonitorResponse::Ptr& command){

}