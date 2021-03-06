//
// Created by scott on 2020-08-14.
//


#include "Direction.h"
#include "GM50/bytearray.h"
#include "Connector.h"
#include "GM50/apps/UserAppJsonMessage.h"

void ChannelDirection::sendData(const char* data,size_t size){

    if( !secure_){
        stream_list.push_back(ByteArray((const char*)data, data+size));
        trySendData();
    }else{
        sendUserAppData(ByteArray(data, data+size));
    }


}

void ChannelDirection::trySendData(){
    if(!connected_){
        return ;
    }
    if(!secure_) {
        for (auto &data : stream_list) {
            conn_->send(data.data(), data.size());
        }
        stream_list.clear();
    }
}

ChannelDirection::Ptr ChannelDirection::against(){
    return connector_->against( shared_from_this());
}

void ChannelDirection::onData(const char * data ,size_t size,const Connection::Ptr& conn){
    // down stream
    // 来自上游服务下发的数据
    ByteArray bytes(data,data+size);
//    down_channel_->sendUserAppData(bytes);
    if(secure_){ // 安全通道
        SecChannel::onData(data,size,conn); // 未协商完成
        if( !channel_okay_){
            return;
        }
        if(latest_command_){ // 解出明文传递给下面的通道
            auto peer = against();
            if(peer){
                auto cmd = std::dynamic_pointer_cast<CommandUserAppJsonMessage>(latest_command_);
                if( cmd ) {
                    peer->sendData(cmd->data.data(),cmd->data.size());
                }
            }
            latest_command_.reset();
        }
    }else{ // 非安全通道
        auto peer = against();
        if(peer) {
            peer->sendData(data,size);
        }
    }

}


ByteArray ChannelDirection::getRemotePubkey(const DeviceID& device_id ){
    if( settings_.remote_pub_key.size()){
        return settings_.remote_pub_key;
    }
    std::stringstream  ss;
    std::string device_sid((const char*)device_id.data(),device_id.size());
    ss << "remote_pub_key_"<< device_sid;
    std::string pub_key = cfgs_.get_string(ss.str(),"");    // read pub_key from settings.txt
    ByteArray key;
    key.assign(pub_key.c_str(),pub_key.c_str()+ pub_key.size());
    return key;
}

ChannelDirection::~ChannelDirection(){
    std::cout<< "~ChannelDirection() .." << std::endl;

}