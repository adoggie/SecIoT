//
// Created by scott on 2020-08-14.
//

#include "UserSecChannel.h"
#include "Controller.h"
#include "ManagedAgent.h"

UserSecChannel::UserSecChannel(){

}

void UserSecChannel::onChannelEstablished(){
    Application::instance()->getLogger().debug("SecChannel Has Established!");
    command_list_.clear();

    ManagedAgent::instance()->onConnected( conn_);
    // 通道建立，发送滞留消息
    channel_okay_ = true;
    trySendData();

}


void UserSecChannel::onCheckHealth() {
    //发送心跳
    auto message = std::make_shared<MessageHeartBeat>();
    message->device_id = Controller::instance()->getDeviceUniqueID();
    this->sendUserAppMessage(message);
}