//
// Created by bin zhang on 2020/7/1.
//

#ifndef BRANCHES_CONNECTION_H
#define BRANCHES_CONNECTION_H

#include "base.h"
#include "connection.h"

#include <memory>
#include <mutex>
#include <list>
#include <vector>
#include <atomic>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <boost/asio/steady_timer.hpp>


enum class ConnectionType{
	SOCKET,
	INNER_DEVICE,

};


class SocketServer;

enum Direction{
	INCOMING,
	OUTGOING
};

enum class ConnectionError{
	Unreachable,
	Timeout
};

struct IConnectionListener;

class  Connection: public std::enable_shared_from_this<Connection>{
//class  Connection{
	std::mutex mutex_;
	std::recursive_mutex rmutex_;
	boost::asio::ip::tcp::socket sock_;	/*!< socket 句柄 */
	SocketServer* server_ = NULL;
	Direction  direction_;
	boost::asio::streambuf streambuf_;
	std::vector<char>   recvbuf_;
	std::shared_ptr<IConnectionListener> listener_;
//	IConnectionListener* listener_;
	std::string id_;
	boost::asio::steady_timer deadline_timer_;
	boost::asio::steady_timer heartbeat_timer_;
	std::size_t  conn_timeout_ = 30 ;    // 连接超时
	std::size_t  read_timeout_ = 0 ;    // 读超时
	std::size_t  heartbeat_=0;          // 心跳
	std::atomic_bool stopped_ ;
	std::atomic_bool disconnected_notify_ ;
	boost::asio::ip::tcp::endpoint ep_;
	void        * user_data_ = NULL ;            // 用户定义数据

	std::vector< std::uint8_t > databuf_;	// data recv buffer
	std::vector<char> databuf_send_; //发送数据缓冲
    char SP_ ='\0';
protected:
	std::string make_id();
public:
	typedef std::shared_ptr<Connection> Ptr;
	friend class  SocketServer;

	Connection( boost::asio::io_service& io_service);
//	Connection(const boost::asio::ip::tcp::endpoint& ep,const boost::asio::io_service& io_service);
	Connection(SocketServer* server);
	virtual  ~Connection();
	void setListener(const std::shared_ptr<IConnectionListener>& listener){
		listener_ = listener;
	}
	SocketServer * server() { return server_;}
	void* userdata() { return user_data_ ;}
	void userdata(void* data) { user_data_ = data ;}

    void setSP(char sp){ SP_ = sp;}
    char getSP(){ return SP_;}

	std::string id(){ return id_;}
	boost::asio::ip::tcp::socket& socket(){
		return sock_;
	}
	
	Connection& heartBeatTime(std::size_t  hbt){
		heartbeat_ = hbt;
		return *this;
	}
	
	Connection& connectTimeout(std::size_t  timeout){
		conn_timeout_ = timeout;
		return *this;
	}
	
	Connection& readTimeout(std::size_t  timeout){
		read_timeout_ = timeout;
		return *this;
	}
	
	virtual void send(const char * data,size_t size);
	
	void send(const std::string& data);
	
	void startConnect(const boost::asio::ip::tcp::endpoint& ep );
	
	void connect();
	void open();
	void close();
	
	void start_read() ;
	
	void start_heartbeat() ;
	void start();
protected:
	
	void check_connect();
	
	void handle_write(const boost::system::error_code& error, size_t bytes_transferred) ;
	
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
//	void handle_read2(const boost::system::error_code& error, size_t bytes_transferred,std::vector<char> bytes );

	void handle_connect(const boost::system::error_code& error) ;

	std::vector<std::string> jsondata_split();
	
};


struct IConnectionListener{ //: std::enable_shared_from_this<IConnectionListener>{
    typedef std::shared_ptr<IConnectionListener> Ptr;
//	virtual void onConnectionReached(Connection::Ptr& conn) = 0;
	virtual void onConnected(const Connection::Ptr& conn){};
	virtual void onDisconnected(const Connection::Ptr& conn){};
	virtual void onData(boost::asio::streambuf& buffer,const Connection::Ptr& conn){};
	virtual void onData(const char * data ,size_t size,const Connection::Ptr& conn){};
	virtual void onJsonText(const std::string & text,const Connection::Ptr& conn){};
	virtual void onConnectError(const Connection::Ptr& conn,ConnectionError error){}  // 0: not reachable , 1: timeout
	
};

#endif //BRANCHES_CONNECTION_H
