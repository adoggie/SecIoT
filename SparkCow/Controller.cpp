
#include "Controller.h"

#include <numeric>
#include <iterator>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <boost/algorithm/string.hpp>


#include "app.h"
#include "utils.h"
#include "Connector.h"
#include "DirectionIn.h"


bool Controller::init(const Config& cfgs){
	boot_time_ = std::time(NULL);
	settings_.login_inited = false;

	cfgs_ = cfgs;
    auto address = cfgs.get_string("listen_ip","127.0.0.1");
    auto port = cfgs.get_int("listen_port",9900);


    server_ = std::make_shared<SocketServer>( this->get_io_service(),address,(unsigned short)port);
//    server_->setListener( std::dynamic_pointer_cast<Controller>( IConnectionListener::shared_from_this()));
    server_->setListener( shared_from_this());

    check_timer_interval_ = 5;
	timer_ = std::make_shared<boost::asio::steady_timer>(Application::instance()->getController()->get_io_service());
	timer_->expires_after(std::chrono::seconds( check_timer_interval_));
	timer_->async_wait(std::bind(&Controller::workTimedTask, this));



    return true;
}

bool Controller::open(){
    server_->start();
	return true;
}


void Controller::close(){

}

void Controller::run(){
    get_io_service().run();
}


void Controller::resetStatus(){
	std::lock_guard<std::recursive_mutex> lock(rmutex_);
//	settings_.login_inited = false;
//	if(conn_){
//		conn_->close();
//		conn_.reset();
//	}
}

// 定时工作任务
void Controller::workTimedTask(){
//	std::lock_guard<std::recursive_mutex> lock(rmutex_);

    timer_->expires_after(std::chrono::seconds( check_timer_interval_));
    timer_->async_wait(std::bind(&Controller::workTimedTask, this));
    return ;

}


void Controller::onConnected(const Connection::Ptr& conn){
   // Application::instance()->getLogger().debug("New  Connection Established!");

    auto connector = std::make_shared<Connector>();
    connector->init(cfgs_);

    connectors_[connector->id()] = connector;
    connector->incoming(conn);
}

//外部连接丢失
void Controller::onDisconnected(const Connection::Ptr& conn){

}

void Controller::onData(const char * data ,size_t size,const Connection::Ptr& conn){

}

void Controller::onConnectError(const Connection::Ptr& conn,ConnectionError error){
}

void Controller::onConnectorClosed(const Connector::Ptr& connector){

    auto itr = connectors_.find(connector->id());
    if( itr!= connectors_.end()) {
        connectors_.erase(itr);
    }
}

std::string Controller::getDeviceUniqueID(){
    return cfgs_.get_string("id");
}