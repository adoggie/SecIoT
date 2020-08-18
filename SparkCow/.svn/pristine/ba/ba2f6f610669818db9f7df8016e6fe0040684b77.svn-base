
#ifndef SECCHAN_CONTROLLER_H
#define SECCHAN_CONTROLLER_H

#include "../base.h"
#include "../service.h"
#include "../connection.h"
#include "../message.h"
#include "../server.h"

#include "../version.h"
#include <jsoncpp/json/json.h>
#include <boost/asio.hpp>

//#include "server_sechannel.h"
#include <memory>
#include <numeric>
#include "../app.h"
#include "../Connector.h"

class Controller : public IController, public Service, public IConnectionListener , public  std::enable_shared_from_this<Controller> {

public:
    struct Settings{
        std::time_t         start_time;          // 启动时间
        std::time_t         login_time;
        std::string         login_server_url;              // 登录服务器url
        std::string         comm_server_ip;         // 通信服务器
        std::uint32_t       comm_server_port;          // 通信服务器端口
        std::time_t         establish_time;         //通信建立时间
        std::atomic_bool    login_inited;           // 是否已经登录
        std::string         token;                  // 接入服务的身份令牌 登录成功时返回
        Settings(){
            start_time = 0 ;
        }
    };
public:
    Controller(){}
	typedef std::shared_ptr<Controller> Ptr;
	bool init(const Config& props);
	bool open();
	void close();
	void run();
	
	static std::shared_ptr<Controller>& instance(){
		static std::shared_ptr<Controller> handle ;
		if(!handle.get()){
			handle = std::make_shared<Controller>() ;
		}
		return handle;
	}


    Settings& settings() { return settings_;};

    void onConnectorClosed(const Connector::Ptr &connector);
    std::string getDeviceUniqueID();;
private:
    void onInitData(const Json::Value& result);
    void workTimedTask();
    void resetStatus();

public:
    friend  class ServerSideSecChannel;
	void onConnected(const Connection::Ptr& conn);
	void onDisconnected(const Connection::Ptr& conn);

    void onData(const char * data ,size_t size,const Connection::Ptr& conn);

	void onConnectError(const Connection::Ptr& conn,ConnectionError error);

protected:
	Config 		cfgs_;
	std::recursive_mutex  rmutex_;
//    boost::asio::io_service io_service_;
    std::shared_ptr<SocketServer>    server_;
//    std::map<std::string,Connection::Ptr> conn_ids_;

    Settings    settings_;
    std::shared_ptr<boost::asio::steady_timer> timer_;
    int         check_timer_interval_;
    bool        data_inited_ ;
    std::time_t     last_check_time_ = 0;
    std::time_t     last_heart_time_ = 0;
    int         net_check_interval_ = 60 ; // 检测与物业和室外机网络连通性时间间隔
//    Connection::Ptr conn_;                  //到平台服务器连接
    std::time_t 	boot_time_ = 0;

    std::map<std::uint64_t , Connector::Ptr>  connectors_;

};


#endif
