//
// Created by scott on 2020-08-14.
//

#include "Connector.h"
#include "app.h"
#include "DirectionIn.h"
#include "DirectionOut.h"
#include "Controller.h"


bool Connector::init(const Config& cfgs){
    id_ = std::time(NULL);

    boot_time_ = std::time(NULL);
    settings_.login_inited = false;

    cfgs_ = cfgs;
    auto address = cfgs.get_string("listen_ip","127.0.0.1");
    auto port = cfgs.get_int("listen_port",9900);
    settings_.in_secure = cfgs.get_string("in_secure") =="true"?true:false;
    settings_.out_secure = cfgs.get_string("out_secure") =="true"?true:false;


//    server_ = std::make_shared<SocketServer>( this->get_io_service(),address,(unsigned short)port);
//    server_->setListener(this);
//
//    check_timer_interval_ = 5;
//    timer_ = std::make_shared<boost::asio::steady_timer>(Application::instance()->getConnector()->get_io_service());
//    timer_->expires_after(std::chrono::seconds( check_timer_interval_));
//    timer_->async_wait(std::bind(&Connector::workTimedTask, this));

    return true;
}

bool Connector::open(){
//    server_->start();
    return true;
}


void Connector::close(){

}

void Connector::run(){
//    get_io_service().run();
}

void Connector::incoming(const Connection::Ptr& conn){
    Application::instance()->getLogger().debug("New  Connection Established!");
//    in_ = std::make_shared<ChannelDirectionIn>(settings_.in_secure,conn,
//            std::dynamic_pointer_cast<Connector>(shared_from_this()));
//
    auto p = std::dynamic_pointer_cast<Connector>(shared_from_this());
    auto in = std::make_shared<ChannelDirectionIn>(settings_.in_secure,p);

                                    //           std::dynamic_pointer_cast<Connector>(shared_from_this()));
//    new ChannelDirectionIn(settings_.in_secure,conn,shared_from_this());
//    new ChannelDirectionIn();
    in->init(cfgs_);
    in->open(conn);
    in_ = in;

}


void Connector::onLostChannel(const SecChannel::Ptr& peer){
//    ChannelDirection::Ptr  other;
//    if( in_ == peer){
//        other = out_;
//    }else{
//        other = in_;
//    }
//    if(other){
//        other->close();
//    }
    if(in_) {
        in_->close();
        in_.reset();
    }
    if(out_) {
        out_->close();
        out_.reset();
    }
    Controller::instance()->onConnectorClosed(shared_from_this());
}

ChannelDirection::Ptr Connector::createNextHop(){
    out_ = std::make_shared<ChannelDirectionOut>( settings_.out_secure,shared_from_this());
    out_->init(cfgs_);
    out_->open();
    return out_;
}

ChannelDirection::Ptr Connector::against(const SecChannel::Ptr& channel){
    if( in_ == channel){
        return out_;
    }
    return in_;
}

std::uint64_t   Connector::id(){
    return id_;
}