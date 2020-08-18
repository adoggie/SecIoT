//
// Created by scott on 2020-08-14.
//

#include "DirectionOut.h"
#include "app.h"
#include "GM50/apps/UserAppJsonMessage.h"
#include "Connector.h"


ChannelDirectionOut::ChannelDirectionOut(bool secure,
        const std::shared_ptr<Connector>& connector):
        ChannelDirection(secure,connector){

}

bool ChannelDirectionOut::init(const Config& cfgs){

    SecChannel::init(cfgs);
    connected_ = false;
    return true;
}

bool ChannelDirectionOut::open(){
    if(!conn_ ){
// 准备发起连接到服务器
        conn_ = std::make_shared<Connection>( Application::instance()->getController()->get_io_service() );
        conn_->setListener( shared_from_this());
        try {
            auto ip = cfgs_.get_string("next_hop_ip");
            auto port = cfgs_.get_int("next_hop_port");
            boost::asio::ip::address address = boost::asio::ip::make_address(ip);
            boost::asio::ip::tcp::endpoint ep(address, (std::uint32_t) port);
            std::stringstream ss;
            ss<<"Connect To Upstream Service:" << ip << ":" << port;
            Application::instance()->getLogger().debug(ss.str());
            conn_->startConnect(ep);
            last_heart_time_ = std::time(NULL);
        }catch(...){
            conn_.reset();
            return false;
        }

    }
    return true;
}

void ChannelDirectionOut::close(){
    if(conn_){
        conn_->close();
        conn_.reset();
    }
}


void ChannelDirectionOut::onConnected(const Connection::Ptr& conn){
    std::cout<< "Upstream Service Connected!" << std::endl;
    connected_  = true;
    conn_ = conn;
    if( secure_){
        SecChannel::onConnected(conn); // 安全通道协商
        return;
    }
    trySendData();
}

void ChannelDirectionOut::onDisconnected(const Connection::Ptr& conn){
    std::cout<< "Upstream Service Lost!" << std::endl;
    connected_ = false;
    destroy();
}



void ChannelDirectionOut::onData(const char * data ,size_t size,const Connection::Ptr& conn){
    ChannelDirection::onData(data,size,conn);
}


void ChannelDirectionOut::onConnectError(const Connection::Ptr& conn,ConnectionError error){
    Application::instance()->getLogger().warn("upstream service can not be reached!");
    destroy();
}


void ChannelDirectionOut::destroy(){
//    auto peer = against();
//    if(peer){
//        peer->close();
//    }
//    conn_.reset();

    connector_->onLostChannel(  shared_from_this());
}

ChannelDirectionOut::~ChannelDirectionOut(){
    std::cout<<"~ChannelDirectionOut()" << std::endl;
}
