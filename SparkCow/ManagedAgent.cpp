//
// Created by scott on 8/4/20.
//

#include "ManagedAgent.h"
#include "GM50/base.h"
#include "GM50/sechannel.h"
#include "bundles/all.h"
#include "app.h"
#include "GM50/sechannel.h"
#include "message.h"
#include "Controller.h"

bool ManagedAgent::init(const Config& cfgs){
    cfgs_ = cfgs;
//    CommandListener::Ptr user = std::make_shared< DeviceManagementImpl >();
//    channel_.setCommandListener( AppID::DeviceManagement, user );
//
//    user = std::make_shared<ComplianceTestImpl>();
//    channel_.setCommandListener( AppID::ComplianceTest, user );
//
//    user = std::make_shared<ParamConfigureImpl>();
//    channel_.setCommandListener( AppID::ParamConfigure, user );
//
//    user = std::make_shared<RemoteMaintenanceImpl>();
//    channel_.setCommandListener( AppID::RemoteMaintenance, user );
//
//    user = std::make_shared<SecKeyManagementImpl>();
//    channel_.setCommandListener( AppID::SecKeyManagement, user );
//
//    user = std::make_shared<VpnManagementImpl>();
//    channel_.setCommandListener( AppID::VpnManagement, user );
//
//    user = std::make_shared<UserAppJsonDataImpl>();
//    channel_.setCommandListener( AppID::UserAppExt, user );
//
//
//    channel_.init(cfgs);
//

    return true;
}

bool ManagedAgent::open(){
//    channel_.open();
    return true;
}

void ManagedAgent::close(){

}


void ManagedAgent::onConnected(const Connection::Ptr& conn){
//    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);

    std::shared_ptr<MessageLogin> login_req = std::make_shared<MessageLogin> ();
    login_req->device_id = Controller::instance()->getDeviceUniqueID();
    login_req->token = cfgs_.get_string("token");

//    channel_.sendUserAppMessage(login_req);
    // 呼叫连接建立，发送登录消息包

}
