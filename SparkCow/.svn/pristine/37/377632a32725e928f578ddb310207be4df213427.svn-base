//
// Created by scott on 2020-08-14.
//

#include "DirectionIn.h"

#include "app.h"
#include "Connector.h"
#include "sdf-utils.h"
#include "GM50/gm0006.h"
#include "connection.h"
#include "Controller.h"

ChannelDirectionIn::ChannelDirectionIn(bool secure,
        const std::shared_ptr<Connector>& connector ):ChannelDirection(secure,connector){
}


void ChannelDirectionIn::onData(const char * data ,size_t size,const Connection::Ptr& conn){
    ChannelDirection::onData(data,size,conn);
}

bool ChannelDirectionIn::init(const Config& props){
//    cfgs_ = props;
    SecChannel::init(cfgs_);
    return true;
}

bool ChannelDirectionIn::open(const Connection::Ptr& conn ){
    connected_ = true;

//    Connection cc(0);
//    auto x = std::make_shared<ChannelDirectionIn>();
//    cc.setListener(x);
    //conn_->setListener( std::dynamic_pointer_cast<IConnectionListener>(shared_from_this()) );
    conn_ = conn;
    conn_->setListener( shared_from_this() );
    onConnected(conn_);
    return true;
}

void ChannelDirectionIn::close(){
    SecChannel::close();
}

void ChannelDirectionIn::onConnected(const Connection::Ptr& conn){
    auto next = connector_->createNextHop();
}

void ChannelDirectionIn::onDisconnected(const Connection::Ptr& conn){
//    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    Application::instance()->getLogger().debug("ChannelDirectionIn::Connection Disconnected..");
    resetStatus();
//    auto peer = this->against();
//    if(peer) {
//        peer->close();
//    }
    connector_->onLostChannel(shared_from_this());
}

// 0: not reachable , 1: timeout
void ChannelDirectionIn::onConnectError(const Connection::Ptr& conn,ConnectionError error){
//    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    resetStatus();
}


void ChannelDirectionIn::resetStatus(){
    SecChannel::resetStatus();

}

void ChannelDirectionIn::onMessageChannelCreateRequest(const MessageChannelCreateRequest::Ptr& message){
//    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    SecChannel::onMessageChannelCreateRequest(message);

}

void ChannelDirectionIn::onMessageChannelCreateResponse(const MessageChannelCreateResponse::Ptr& message){
    SecChannel::onMessageChannelCreateResponse(message);
}

void ChannelDirectionIn::onMessageChannelSend(const MessageChannelSend::Ptr& message){
    SecChannel::onMessageChannelSend(message);

//    SecChannel::onMessageChannelSend(message);
//    if( message->detail->app_id == AppID::UserAppExt ){
//        auto m = std::dynamic_pointer_cast<CommandUserAppJsonMessage>(message->detail);
//        if( !upstream_){
//            upstream_ = std::make_shared<UpstreamService>();
//            upstream_->init( this->shared_from_this(), cfgs_);
//            if( !upstream_->open() ){
//                std::cout<< "Upstream Open Failed!"<< std::endl;
//                this->close();
//            }
//        }
//        upstream_->redirectData( m->data );
//        std::cout << "redirect data:" << m->data.size() << std::endl ;
//        std::for_each(m->data.begin(),m->data.end(),[](int v){
//            std::cout<< "0x"<< std::hex<< v << "," ;
//        });
//        std::cout<< std::endl;
//
//    }
}

void ChannelDirectionIn::onMessageChannelReCreateRequest(const MessageChannelReCreateRequest::Ptr& message){

}

ChannelDirectionIn::~ChannelDirectionIn(){
    std::cout<<"~ChannelDirectionIn().." << std::endl;

}


