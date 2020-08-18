//
// Created by scott on 8/4/20.
//

#include "DeviceManagementImpl.h"


void DeviceManagementImpl::onCommand(const Command::Ptr& command){
    {
        CommandGet::Ptr  cp = std::dynamic_pointer_cast<CommandGet>( command);
        if( cp.get()){
            onCommandGet(cp);
            return;
        }
    }
    {
        CommandGetNext::Ptr  cp = std::dynamic_pointer_cast<CommandGetNext>( command);
        if( cp.get()){
            onCommandGetNext(cp);
            return;
        }
    }
    {
        CommandGetBulk::Ptr  cp = std::dynamic_pointer_cast<CommandGetBulk>( command);
        if( cp.get()){
            onCommandGetBulk(cp);
            return;
        }
    }
    {
        CommandSet::Ptr  cp = std::dynamic_pointer_cast<CommandSet>( command);
        if( cp.get()){
            onCommandSet(cp);
            return;
        }
    }
    {
        CommandResponse::Ptr  cp = std::dynamic_pointer_cast<CommandResponse>( command);
        if( cp.get()){
            onCommandResponse(cp);
            return;
        }
    }
    {
        CommandInform::Ptr  cp = std::dynamic_pointer_cast<CommandInform>( command);
        if( cp.get()){
            onCommandInform(cp);
            return;
        }
    }

    {
        CommandTrap::Ptr  cp = std::dynamic_pointer_cast<CommandTrap>( command);
        if( cp.get()){
            onCommandTrap(cp);
            return;
        }
    }

}


void DeviceManagementImpl::onCommandGet(const CommandGet::Ptr& command){

}

void DeviceManagementImpl::onCommandGetNext(const CommandGetNext::Ptr& command){

}

void DeviceManagementImpl::onCommandGetBulk(const CommandGetBulk::Ptr& command){


}


void DeviceManagementImpl::onCommandResponse(const CommandResponse::Ptr& command){

}


void DeviceManagementImpl::onCommandSet(const CommandSet::Ptr& command){


}


void DeviceManagementImpl::onCommandInform(const CommandInform::Ptr& command){


}


void DeviceManagementImpl::onCommandTrap(const CommandTrap::Ptr& command){

}


