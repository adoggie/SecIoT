//
// Created by bin zhang on 2019/7/1
//
#include "connection.h"
#include "server.h"
#include "utils.h"

#include <memory>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

Connection::Connection( boost::asio::io_service& io_service):
        sock_(io_service),deadline_timer_(io_service),heartbeat_timer_(io_service){
    id_ = make_id();
    stopped_ = false;
    disconnected_notify_ = false;
}

Connection::~Connection(){
    std::cout<< "socket ~connection().. " << std::endl;
}

Connection::Connection(SocketServer* server):
        sock_(server->get_io_service()),
        deadline_timer_(server->get_io_service()),
        heartbeat_timer_(server->get_io_service()),
        server_(server){
    direction_ = Direction::OUTGOING;
    id_ = make_id();
    stopped_ = false;
    disconnected_notify_ = false;
}

std::string Connection::make_id(){
    return utils::generateUUID();
}

std::vector<std::string> Connection::jsondata_split(){
//    const char SP ='\0';
//	const char SP ='|';
    char SP = SP_;
    std::vector<std::string> lines;
    std::string data((const char*)&databuf_[0],databuf_.size());
    puts("<<-");
    puts(data.c_str());
    while(true) {
        auto itr = std::find(databuf_.begin(), databuf_.end(), SP);
        if (itr == databuf_.end()) {
            break;
        }
        std::string text(databuf_.begin(),itr) ;
        databuf_.erase(databuf_.begin(),itr+1);
        lines.push_back(text);
    }
    return lines;
}

void Connection::handle_read(const boost::system::error_code& error, size_t bytes_transferred){
    const char SP ='\0';
    if (stopped_) {
        return;
    }
//    if(!sock_.is_open()){ // todo
//        return;
//    }

    if (!error) {
        std::cout << bytes_transferred << " bytes have read "  << std::endl;
        listener_->onData( (const char*)recvbuf_.data(), bytes_transferred,shared_from_this());
        start_read();
    }else {
        if(!disconnected_notify_ ) {
            std::cout << "socket connection lost." << std::endl;
            std::cerr << error.message() << std::endl;

            if (listener_) {
                listener_->onDisconnected(shared_from_this());
            }

            deadline_timer_.cancel();
            heartbeat_timer_.cancel();
            if (sock_.is_open()) {
                sock_.close();
            }
            disconnected_notify_ = true;
        }

    }

}

void Connection::open(){

}

void Connection::start() {
    // Start the input actor.
    recvbuf_.resize(1024);
    start_read();
}


void Connection::send(const char * data,size_t size){
	SCOPED_LOCK
    boost::system::error_code ec;
    boost::asio::write(sock_, boost::asio::buffer(data,size),
            boost::asio::transfer_all(),ec);
}

void Connection::send(const std::string& data){
    send(data.c_str(),data.size()); // 发送包含 \0 , 这个方式很不通用哦，这里将就一下
    char p[1] ={SP_};
    send(p,1);
}

void Connection::startConnect(const boost::asio::ip::tcp::endpoint& ep ){
    ep_ = ep;
    connect();
}

void Connection::connect(){
    deadline_timer_.expires_after(std::chrono::seconds(conn_timeout_));
    deadline_timer_.async_wait(std::bind(&Connection::check_connect, this));
    sock_.async_connect(ep_,
                        boost::bind(&Connection::handle_connect,
                                    this, _1));
}


void Connection::close(){

    if(stopped_) {
        return;
    }
    boost::system::error_code ignored_error;
    sock_.close(ignored_error);
}

void Connection::start_read() {
    // Set a deadline for the read operation.
    if(read_timeout_) {
        deadline_timer_.expires_after(std::chrono::seconds(read_timeout_));
    }
    sock_.async_read_some(boost::asio::buffer(recvbuf_.data(),recvbuf_.size()), boost::bind(&Connection::handle_read, shared_from_this(),
                                      boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}


void Connection::start_heartbeat() {
    if (stopped_)
        return;

    // Start an asynchronous operation to send a heartbeat message.
    heartbeat_timer_.expires_after(std::chrono::seconds(read_timeout_));
    heartbeat_timer_.async_wait(std::bind(&Connection::start_heartbeat, this));
}

//连接超时检测
void Connection::check_connect(){
    if(stopped_){
        return;
    }
    if(deadline_timer_.expiry() <= boost::asio::steady_timer::clock_type::now()){
        if(!sock_.is_open()){
            sock_.close() ; // 超时了关闭socket
        }
    }
}

void Connection::handle_write(const boost::system::error_code& error, size_t bytes_transferred) {
//    SCOPED_LOCK
    std::lock_guard<std::recursive_mutex> lock(rmutex_);

    if (stopped_) {
        return;
    }

    if (!error) {
        if(bytes_transferred>0){
            if(databuf_send_.size() >= bytes_transferred){
                databuf_send_.erase(databuf_send_.begin(),databuf_send_.begin()+bytes_transferred);
            }
        }
        std::cout<<"Socket databuf_send size: "<< databuf_send_.size()<<std::endl;
        if( databuf_send_.size() > 0){
            boost::asio::async_write(sock_, boost::asio::buffer(&databuf_send_[0],databuf_send_.size()),
                                     boost::bind(&Connection::handle_write, shared_from_this(),
                                                 boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

        }else{

        }
    } else {
        std::cout << "Error on writing: " << error.message() << "\n";
    }
}

void Connection::handle_connect(const boost::system::error_code& error) {
    if (stopped_)
        return;

    if (!sock_.is_open()) {
        std::cout << "Connect timed out\n";
        if(listener_){
            listener_->onConnectError(shared_from_this(),ConnectionError::Timeout);
        }
    } else if (error) { // Check if the connect operation failed before the deadline expired.
        std::cout << "Connect error: " << error.message() << "\n";
        sock_.close();
        if(listener_){
            listener_->onConnectError(shared_from_this(),ConnectionError::Unreachable);
        }
    } else { // Otherwise we have successfully established a connection.
        if(listener_){
            listener_->onConnected(shared_from_this());
        }
        start();
    }
}