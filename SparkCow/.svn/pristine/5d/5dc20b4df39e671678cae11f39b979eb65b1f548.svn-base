//
// Created by scott on 2020-08-14.
//

#ifndef SMARTBOX_CONNECTOR_H
#define SMARTBOX_CONNECTOR_H

#include "base.h"
#include "Direction.h"
#include "connection.h"
#include "config.h"


class Connector: public std::enable_shared_from_this<Connector> {
public:
    typedef std::shared_ptr<Connector> Ptr;
    struct Settings{
        std::time_t         start_time;          // 启动时间
        std::time_t         login_time;
        std::string         login_server_url;              // 登录服务器url
        std::string         comm_server_ip;         // 通信服务器
        std::uint32_t       comm_server_port;          // 通信服务器端口
        std::time_t         establish_time;         //通信建立时间
        std::atomic_bool    login_inited;           // 是否已经登录
        std::string         token;                  // 接入服务的身份令牌 登录成功时返回
        bool    in_secure; //  进入是否加密
        bool    out_secure; //
        Settings(){
            start_time = 0 ;
        }
    };


    bool init(const Config& props);
    bool open();
    void close();
    void run();

    std::uint64_t   id();

    ChannelDirection::Ptr against(const SecChannel::Ptr& channel);

    ChannelDirection::Ptr createNextHop();


    virtual void incoming(const Connection::Ptr& conn);
    Config& getConfig(){ return cfgs_;}
    Settings& getSettings() { return settings_;}

    virtual void onLostChannel(const SecChannel::Ptr& peer);
protected:
//    void onConnected(const Connection::Ptr& conn);
//    void onDisconnected(const Connection::Ptr& conn);
//
//    void onData(const char * data ,size_t size,const Connection::Ptr& conn);
//
//    void onConnectError(const Connection::Ptr& conn,ConnectionError error);
protected:
    ChannelDirection::Ptr  in_;
    ChannelDirection::Ptr  out_;

    Config 		cfgs_;
    std::recursive_mutex  rmutex_;

    Settings    settings_;
    std::shared_ptr<boost::asio::steady_timer> timer_;
    int         check_timer_interval_;
    bool        data_inited_ ;
    std::time_t 	boot_time_ = 0;

    std::uint64_t  id_;

};


#endif //SMARTBOX_CONNECTOR_H
