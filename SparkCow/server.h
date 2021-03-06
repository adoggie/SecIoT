
#ifndef INNERPROC_SERVER_H
#define INNERPROC_SERVER_H

#include <string>
#include <memory>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "connection.h"
#include "service.h"


class ListenAddress{

};


class ListenServer{};


class SocketServer{

	boost::asio::ip::tcp::socket sock_;
	std::array<char,1024>   data_;
	std::string             ip_;
	unsigned short          port_;
	std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
	IConnectionListener::Ptr listener_;
//	IConnectionListener* listener_;
public:
	SocketServer( boost::asio::io_service& io_service,const std::string& ip, const unsigned short port):sock_(io_service){
		acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(io_service);
		ip_ = ip ;
		port_ = port;
	}

	bool start();
	void setListener( const IConnectionListener::Ptr listener){
		listener_ = listener;
	}

	IConnectionListener::Ptr getListener(){
		return listener_;
	}
	
	boost::asio::io_service & get_io_service(){
		return acceptor_->get_executor().context();
	}

	void close(){
	
	}
protected:
	void start_accept();
	void handle_accept(Connection::Ptr  new_connection, const boost::system::error_code& error);
	
protected:

};



#endif
