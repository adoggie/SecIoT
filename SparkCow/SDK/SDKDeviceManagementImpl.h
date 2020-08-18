//
// Created by scott on 2020-08-17.
//

#ifndef SMARTBOX_SDKDEVICEMANAGEMENTIMPL_H
#define SMARTBOX_SDKDEVICEMANAGEMENTIMPL_H

#include "../bundles/DeviceManagementImpl.h"
#include "ContextConnection.h"
#include "SDKDeviceManagementImpl.h"
class SDKDeviceManagementImpl:public DeviceManagementImpl {
public:
    typedef std::shared_ptr<SDKDeviceManagementImpl> Ptr;
    SDKDeviceManagementImpl(const InProcDirectionIn::Ptr& direction){
        direction_ = direction;
    }

    virtual void onCommandGet(const CommandGet::Ptr& command);
    virtual void onCommandGetNext(const CommandGetNext::Ptr& command);
    virtual void onCommandGetBulk(const CommandGetBulk::Ptr& command);
    virtual void onCommandResponse(const CommandResponse::Ptr& command);
    virtual void onCommandSet(const CommandSet::Ptr& command);
    virtual void onCommandInform(const CommandInform::Ptr& command);
    virtual void onCommandTrap(const CommandTrap::Ptr& command);
protected:
    InProcDirectionIn::Ptr  direction_;
};


#endif //SMARTBOX_SDKDEVICEMANAGEMENTIMPL_H
