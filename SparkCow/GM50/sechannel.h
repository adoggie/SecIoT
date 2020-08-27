//
// Created by scott on 7/29/20.
//

#ifndef MONKEY_SECHANNEL_H
#define MONKEY_SECHANNEL_H

#include "message.h"
#include "../config.h"
#include "../connection.h"
#include "command.h"
#include <map>
#include <memory>
#include <atomic>
#include "envelope.h"
#include "../message.h"



class SecChannel:public IConnectionListener,public std::enable_shared_from_this<SecChannel> {
    DECL_SINGLETON(SecChannel)
public:
    typedef std::shared_ptr<SecChannel> Ptr;
    struct Settings{
//        std::time_t         start_time;          // 启动时间
//        std::time_t         login_time;
//        std::string         login_server_url;              // 登录服务器url
        std::string         comm_server_ip;         // 通信服务器
        std::uint32_t       comm_server_port;          // 通信服务器端口
//        std::time_t         establish_time;         //通信建立时间
//        std::atomic_bool    login_inited;           // 是否已经登录
//        std::string         token;                  // 接入服务的身份令牌 登录成功时返回

        ByteArray       remote_pub_key;       //
        ByteArray       local_pub_key_sign;
        ByteArray       local_pub_key_enc;
        ByteArray       local_pri_key;
        ByteArray       remote_id; // manage center 's id
        ByteArray       local_id ; // agent's id

        ByteArray       temp_key;   // 临时会话密钥 local make
        ByteArray       temp_peer_key;   // 临时会话密钥 对端返回的临时密钥

//        ByteArray       session_key;


        Settings(){
//            start_time = 0 ;
        }
    };

    SecChannel();
    ~SecChannel();
    virtual bool init(const Config& cfgs);
    virtual bool open();
    virtual void close();
    virtual void initCommandListener();
public:

    void onConnected(const Connection::Ptr& conn);
    void onDisconnected(const Connection::Ptr& conn);
    void onData(const char * data ,size_t size,const Connection::Ptr& conn);
    virtual void onConnectError(const Connection::Ptr& conn,ConnectionError error);  // 0: not reachable , 1: timeout
protected:
    void onError();

    virtual void onMessageChannelCreateRequest(const MessageChannelCreateRequest::Ptr& message);
    virtual void onMessageChannelCreateResponse(const MessageChannelCreateResponse::Ptr& message);
    virtual void onMessageChannelSend(const MessageChannelSend::Ptr& message);
    virtual void onMessageChannelReCreateRequest(const MessageChannelReCreateRequest::Ptr& message);

    virtual void onChannelEstablished();    // 通道已建立
    virtual void trySendData();
public:
    bool sendCommand(const Command::Ptr& command);

    void setCommandListener( AppID app_id, CommandListener::Ptr& user);
    void setTempKey(const ByteArray& key) {  settings_.temp_peer_key = key;}
    ByteArray &  getTempKey(){ return settings_.temp_peer_key;}
    Settings& getSettings(){ return settings_; }

    virtual ByteArray getRemotePubkey(const DeviceID& device_id ) { return settings_.remote_pub_key;}

    virtual void sendUserAppMessage(const std::shared_ptr<MessageTraverse> message); // via user channel
    virtual void sendUserAppData(const ByteArray& data);

    SecChannel& setConnection(const Connection::Ptr& conn){ conn_ = conn; return *this;}
    Connection::Ptr getConnection(){ return conn_;}
//    boost::asio::io_service& get_io_service(){
//        return io_service_;
//    }

    void setLatestCommand(const Command::Ptr& command){  latest_command_ = command;}
protected:
    bool createChannelRequest();
    bool createChannelResponse();
    bool reCreateChannelRequest();

    void workTimedTask();

    void resetStatus();

    virtual void sendData(const char * data, size_t size);
    void sendEnvelope(const Envelope::Ptr& evp)  ;
    Envelope::Ptr prepareEnvelope(const MessageChannelTraverse::Ptr& message);
    virtual void onCheckHealth();

protected:
    Config 		cfgs_;
    std::recursive_mutex  rmutex_;
//    boost::asio::io_service io_service_;
    Settings    settings_;
    std::shared_ptr<boost::asio::steady_timer> timer_;
    int         check_timer_interval_;
    bool        data_inited_ ;
    std::time_t     last_check_time_ = 0;
    std::time_t     last_heart_time_ = 0;
    int         net_check_interval_ = 60 ; // 检测与物业和室外机网络连通性时间间隔
    Connection::Ptr conn_;                  //到平台服务器连接
    std::time_t 	boot_time_ = 0;
    EnvelopeManager evp_mgr_ ;
protected:
//    std::vector< CommandListener::Ptr >  command_users_;
    std::map< AppID, CommandListener::Ptr > app_users_;
    std::atomic_bool    channel_okay_;
    std::list< Command::Ptr> command_list_;
    std::time_t     test_time_;

    Command::Ptr  latest_command_;
};


#endif //MONKEY_SECHANNEL_H
