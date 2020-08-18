//
// Created by scott on 8/4/20.
//

#ifndef MONKEY_DEVICEMANAGEMENTIMPL_H
#define MONKEY_DEVICEMANAGEMENTIMPL_H


#include "../GM50/command.h"

class DeviceManagementImpl:public CommandListener {
public:
    typedef std::shared_ptr<DeviceManagementImpl> Ptr;

protected:
    virtual void onCommand(const Command::Ptr& command);
    virtual void onCommandGet(const CommandGet::Ptr& command);
    virtual void onCommandGetNext(const CommandGetNext::Ptr& command);
    virtual void onCommandGetBulk(const CommandGetBulk::Ptr& command);
    virtual void onCommandResponse(const CommandResponse::Ptr& command);
    virtual void onCommandSet(const CommandSet::Ptr& command);
    virtual void onCommandInform(const CommandInform::Ptr& command);
    virtual void onCommandTrap(const CommandTrap::Ptr& command);
};


#endif //MONKEY_DEVICEMANAGEMENTIMPL_H
