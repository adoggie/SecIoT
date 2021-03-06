//
// Created by scott on 7/29/20.
//

#include <memory>
#include <iterator>
#include <algorithm>
#include "sechannel.h"
#include "envelope.h"
#include "../app.h"
#include "../sdf-utils.h"
#include "gm0006.h"
#include "apps/UserAppJsonMessage.h"
//#include "../ManagedAgent.h"
#include "../message.h"
#include "../bundles/all.h"
#include <boost/beast/core/detail/base64.hpp>




void SecChannel::onData(const char * data ,size_t size,const Connection::Ptr& conn){
    int error;
    size_t consumed = 0;
    EnvelopeArray evps ;
    int  err = evp_mgr_.enqueue(data,size , evps);
    if( err ){
        onError();
        return;
    }
    for(auto & evp: evps ){

        if( !evp->detail.get()){
            continue;
        }
        {
            MessageChannelCreateRequest::Ptr message = std::dynamic_pointer_cast<MessageChannelCreateRequest>( evp->detail);
            if(message.get()){
                this->onMessageChannelCreateRequest(message);
            }
        }
        {
            MessageChannelCreateResponse::Ptr message = std::dynamic_pointer_cast<MessageChannelCreateResponse>( evp->detail);
            if(message.get()){
                this->onMessageChannelCreateResponse(message);
            }
        }
        {
            MessageChannelSend::Ptr message = std::dynamic_pointer_cast<MessageChannelSend>(evp->detail);
            if(message.get()){
                this->onMessageChannelSend(message);
            }
        }
        {
            MessageChannelReCreateRequest::Ptr message = std::dynamic_pointer_cast<MessageChannelReCreateRequest>(evp->detail);
            if(message.get()){
                this->onMessageChannelReCreateRequest(message);
            }
        }
    }
}


void SecChannel::onError(){
    resetStatus();
}

void SecChannel::onMessageChannelCreateRequest(const MessageChannelCreateRequest::Ptr& message){
    settings_.remote_id = message->envelope->source; // 远端客户设备编号 32 bytes
    // to match remote's pub-key
    // 根据连接设备编号找到对点设备的公钥

    settings_.remote_pub_key= this->getRemotePubkey(settings_.remote_id);

    // 使用本地私钥解出 临时共享密钥

    sdf::data_dec(sdf::KeyID(0),SGD_SM2,settings_.local_pri_key.data(),settings_.local_pri_key.size(),
                  message->enc_data.data(),message->enc_data.size(), settings_.temp_peer_key);

    ////////  response to peer
    sdf::random_create(settings_.temp_key,32);

    auto response = std::make_shared<MessageChannelCreateResponse>();
    {
        ByteArray data; //= settings_.temp_peer_key+ settings_.temp_key;
        std::copy(settings_.temp_peer_key.begin(), settings_.temp_peer_key.end(), std::back_inserter(data));
        std::copy(settings_.temp_key.begin(), settings_.temp_key.end(), std::back_inserter(data));
        sdf::data_enc(sdf::KeyID(0),SGD_SM2,settings_.remote_pub_key.data(), settings_.remote_pub_key.size(),
                      data.data(), data.size(),response->enc_data);
    }
    {
        sdf::data_sign(sdf::KeyID(0),SGD_SM2,settings_.local_pri_key.data(),settings_.local_pri_key.size(),
                       settings_.temp_key.data(),settings_.temp_key.size(),
                       response->sign_data
        );
    }

    auto evp = this->prepareEnvelope(response);
    sendEnvelope(evp);

    channel_okay_ = true;
}

void SecChannel::onMessageChannelCreateResponse(const MessageChannelCreateResponse::Ptr& message){
    // got response , ready for sending
    // 服务器返回对点 会话密钥
    ByteArray key_b;
    {
        // 解密出 远端 生成的临时密钥 B
        ByteArray key_ab;
        sdf::data_dec(sdf::KeyID(0), SGD_SM2, settings_.local_pri_key.data(), settings_.local_pri_key.size(),
                      message->enc_data.data(), message->enc_data.size(),
                      key_ab
        );
        // key = a + b

        std::copy_n(key_ab.begin() + settings_.temp_key.size(), key_ab.size() - settings_.temp_key.size(),
                    std::back_inserter(key_b));
        settings_.temp_peer_key = key_b; // 远端server 's 动态密钥
    }
    // verify  服务器公钥验签名
    {
        sdf::data_verify(sdf::KeyID(0),SGD_SM2,settings_.remote_pub_key.data(),settings_.remote_pub_key.size(),
                         key_b.data(),key_b.size(),
                         message->sign_data.data(),message->sign_data.size()
        );

    }
    onChannelEstablished();
}

void SecChannel::onMessageChannelSend(const MessageChannelSend::Ptr& message){
    for(auto& p :  app_users_){
        if( p.first == message->detail->app_id ){
            p.second->onCommand( message-> detail );
            break;
        }
    }
}

void SecChannel::onMessageChannelReCreateRequest(const MessageChannelReCreateRequest::Ptr& message){

}


void SecChannel::onChannelEstablished(){
    Application::instance()->getLogger().debug("SecChannel Has Established!");
    command_list_.clear();

    // 通道建立，发送滞留消息
    channel_okay_ = true;
    trySendData();
}

void SecChannel::trySendData(){
    if( !channel_okay_){
        return ;
    }
    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    for(auto& command : command_list_){
        MessageChannelSend::Ptr message = std::make_shared<MessageChannelSend>();
        message->detail = command;
        auto evp = this->prepareEnvelope(message);
        sendEnvelope(evp);
    }
    command_list_.clear();
}

SecChannel::SecChannel(){
    evp_mgr_.setSecChannel( this);
    channel_okay_ = false;
}

SecChannel::~SecChannel(){
//    std::cout<< "~SecChannel().."<< std::endl;
    conn_.reset();
}

bool SecChannel::init(const Config& props){
    cfgs_ = props;

    settings_.comm_server_ip = cfgs_.get_string("server_ip","127.0.0.1");
    settings_.comm_server_port = cfgs_.get_int("server_port",9801);
    settings_.remote_id.resize(32);
    settings_.local_id.resize(32);
    std::string  value ;
    value = cfgs_.get_string("remote_pub_key","");

    std::copy( value.begin(),value.begin()+value.size(),std::back_inserter(settings_.remote_pub_key));

    value = cfgs_.get_string("local_pri_key","");
    std::copy( value.begin(),value.begin()+value.size(),std::back_inserter(settings_.local_pri_key));

    value = cfgs_.get_string("local_pub_key_sign");
    std::copy( value.begin(),value.begin()+value.size(),std::back_inserter(settings_.local_pub_key_sign));
    {
        ByteArray bytes;
        bytes.resize(boost::beast::detail::base64::decoded_size(settings_.local_pub_key_sign.size()));
        auto const result = boost::beast::detail::base64::decode(
                &bytes[0], settings_.local_pub_key_sign.data(), settings_.local_pub_key_sign.size());
        settings_.local_pub_key_sign = bytes;
    }


    value = cfgs_.get_string("local_pub_key_enc");
    std::copy( value.begin(),value.begin()+value.size(),std::back_inserter(settings_.local_pub_key_enc));
    {
        ByteArray bytes;
        bytes.resize(boost::beast::detail::base64::decoded_size(settings_.local_pub_key_enc.size()));
        auto const result = boost::beast::detail::base64::decode(
                &bytes[0], settings_.local_pub_key_enc.data(), settings_.local_pub_key_enc.size());
        settings_.local_pub_key_enc = bytes;
    }

    value = cfgs_.get_string("remote_id");
    std::copy_n( value.begin(),value.size(),settings_.remote_id.begin());

    value = cfgs_.get_string("local_id");
    std::copy_n( value.begin(),value.size(),settings_.local_id.begin());


    resetStatus();

    check_timer_interval_ = cfgs_.get_int("controller.check_timer",5);
    if( check_timer_interval_ < 30){
//        check_timer_interval_ = 30;
    }

//    timer_ = std::make_shared<boost::asio::steady_timer>( Application::instance()->getController()->get_io_service());
//    timer_->expires_after(std::chrono::seconds( check_timer_interval_));
//    timer_->async_wait(std::bind(&SecChannel::workTimedTask, this));

    this->initCommandListener();

    return true;
}

void SecChannel::initCommandListener(){
    CommandListener::Ptr user = std::make_shared< DeviceManagementImpl >();
    setCommandListener( AppID::DeviceManagement, user );

    user = std::make_shared<ComplianceTestImpl>();
    setCommandListener( AppID::ComplianceTest, user );

    user = std::make_shared<ParamConfigureImpl>();
    setCommandListener( AppID::ParamConfigure, user );

    user = std::make_shared<RemoteMaintenanceImpl>();
    setCommandListener( AppID::RemoteMaintenance, user );

    user = std::make_shared<SecKeyManagementImpl>();
    setCommandListener( AppID::SecKeyManagement, user );

    user = std::make_shared<VpnManagementImpl>();
    setCommandListener( AppID::VpnManagement, user );

    user = std::make_shared<UserAppJsonDataImpl>();
    setCommandListener( AppID::UserAppExt, user );
}

bool SecChannel::open(){
    return true;
}

void SecChannel::close(){
    if(conn_){
        conn_->close();
    }
}


// 一旦连接对点，发送通道创建请求
// 生成会话密钥K,并用对点PubKey进行加密并传输
void SecChannel::onConnected(const Connection::Ptr& conn){
    Application::instance()->getLogger().debug("Peer Connected..");

    conn_ = conn;
    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);


    auto message = std::make_shared<MessageChannelCreateRequest>();
    // 产生临时密钥
    sdf::random_create(this->settings_.temp_key,32);
    // 对点公钥加密
    sdf::data_enc(sdf::KeyID(0),SGD_SM2,this->settings_.local_pub_key_enc.data(),this->settings_.local_pub_key_enc.size(),
            settings_.temp_key.data(), settings_.temp_key.size(),
            message->enc_data);
    // 自己私钥签名
    sdf::data_sign(sdf::KeyID(0),SGD_SM2,settings_.local_pri_key.data(), settings_.local_pri_key.size(),
            settings_.temp_key.data(), settings_.temp_key.size(),
            message->sign_data
            );

    auto evp = this->prepareEnvelope(message);
    sendEnvelope(evp);

}

void SecChannel::onDisconnected(const Connection::Ptr& conn){
//    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    Application::instance()->getLogger().debug("SecChannel::Connection Disconnected..");
    resetStatus();
}

// 0: not reachable , 1: timeout
void SecChannel::onConnectError(const Connection::Ptr& conn,ConnectionError error){
//    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    resetStatus();
}


void SecChannel::setCommandListener( AppID app_id, CommandListener::Ptr& user){
    app_users_[app_id] = user;
    user->channel = this;
}


// 定时工作任务
void SecChannel::workTimedTask(){
//    std::lock_guard<std::recursive_mutex> lock(rmutex_);

//	if(!conn_ && settings_.login_inited){
//    if(!conn_ ){
        if(!channel_okay_){
        // 准备发起连接到服务器
        conn_ = std::make_shared<Connection>( Application::instance()->getController()->get_io_service());
        conn_->setListener( shared_from_this() );
        boost::asio::ip::address address = boost::asio::ip::make_address(settings_.comm_server_ip);
        boost::asio::ip::tcp::endpoint ep(address,(std::uint32_t )settings_.comm_server_port);
        std::stringstream ss;
        ss<<"Connect to server:" << settings_.comm_server_ip << ":" << settings_.comm_server_port;
        Application::instance()->getLogger().debug(ss.str());
        conn_->startConnect(ep);
        last_heart_time_ = std::time(NULL);
    }

    // 如果超时接收不到来自服务器的心跳包，则关闭连接，再次进行登录
    if( std::time(NULL) - last_heart_time_ > 60*1){
//        resetStatus();
    }

    if(0)
    if( std::time(nullptr) - test_time_ > 5){
        test_time_ = std::time(nullptr);
        auto cmd = std::make_shared<CommandUserAppJsonMessage>();
        cmd->data.resize(10);
        std::fill_n(cmd->data.begin(),10,'X');
        this->sendCommand(cmd);
    }

    if(channel_okay_){
        onCheckHealth();
    }

    timer_->expires_after(std::chrono::seconds( check_timer_interval_));
    timer_->async_wait(std::bind(&SecChannel::workTimedTask, this));

}


void SecChannel::onCheckHealth(){

}

void SecChannel::resetStatus(){
//    std::lock_guard<std::recursive_mutex> lock(rmutex_);
//    settings_.login_inited = false;
    if(conn_){
        conn_->close();
        conn_.reset();
    }
    evp_mgr_.reset();
    channel_okay_ = false;
    for(auto& user: app_users_){
        user.second->reset();
    }

}

void SecChannel::sendData(const char * data, size_t size){
//    std::lock_guard<std::recursive_mutex> lock(rmutex_);
    if(conn_){
        conn_->send(data,size);
    }
}

void SecChannel::sendEnvelope(const Envelope::Ptr& evp)  {
    ByteArray  data,sign;
    ByteStream final ;
    evp->marshall(final);
    data.clear();
    Application::instance()->getLogger().debug("Sending Envelope..");

    this->SecChannel::sendData( final.data() , final.size());
}

////
// 1. allocate message
// 2. setup envelope with message
// 3. serialize it and send it out via connection
bool SecChannel::createChannelRequest() {
    MessageChannelCreateRequest::Ptr message = std::make_shared<MessageChannelCreateRequest>();
    ByteArray  random;
    ByteArray  data;

    sdf::random_create(random,32);
    auto & settings = SecChannel::getSettings();
            settings.temp_key = random;

    sdf::data_enc(sdf::KeyID (0),SGD_SM2, settings.remote_pub_key.data(),settings.remote_pub_key.size(),
            random.data(),random.size(),data);
    message->enc_len = data.size();
    message->enc_data = data ;

    sdf::data_sign(sdf::KeyID(0), SGD_SM2, settings.local_pri_key.data(),settings.local_pri_key.size(),
            random.data(),random.size(),message->sign_data);
    message->sign_len = message->sign_data.size();

    auto evp = prepareEnvelope(message);
    sendEnvelope(evp);

    return true;
}

Envelope::Ptr SecChannel::prepareEnvelope(const MessageChannelTraverse::Ptr& message){

    ByteArray  random;
    ByteArray  data;

    auto evp = evp_mgr_.createEnvelope();
    evp->pdu_len = message->getSize();
    evp->target = settings_.remote_id;
    evp->source = settings_.local_id;
    evp->op_type = message->type;
    evp->detail = message;

    evp->channel = this;
    message->channel = this;
    return evp;
}

bool SecChannel::sendCommand(const Command::Ptr& command){
//    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    Application::instance()->getLogger().debug("Push Command Into Sending Pool..");
    command_list_.push_back(command);
    SecChannel::trySendData();
    return true;
}

bool SecChannel::createChannelResponse(){
    auto message = std::make_shared<MessageChannelCreateResponse>();

    ByteArray  random;
    ByteArray  data;

    sdf::random_create(random,32);
//    sdf::sm2_enc(settings_.remote_pub_key,random.data(),random.size(),data);
    message->enc_len = data.size();
    message->enc_data = data ;

//    sdf::sm2_sign(settings_.local_pri_key,random.data(),random.size(),message->sign_data);
    message->sign_len = message->sign_data.size();

    auto evp = prepareEnvelope(message);
    sendEnvelope(evp);

    return true;
}

bool SecChannel::reCreateChannelRequest(){
    MessageChannelReCreateRequest::Ptr message = std::make_shared<MessageChannelReCreateRequest>();
    auto evp = prepareEnvelope(message);
    sendEnvelope(evp);
    return true;
}

void SecChannel::sendUserAppMessage(const std::shared_ptr<MessageTraverse> message){
    std::string json = message->marshall();
    auto cp = std::make_shared<CommandUserAppJsonMessage>();
    cp->data.assign(json.c_str(),json.c_str()+json.size());
    cp->data.push_back('\0'); // json 消息的分割符
    this->sendCommand(cp);
}

void SecChannel::sendUserAppData(const ByteArray& data){
    auto cp = std::make_shared<CommandUserAppJsonMessage>();
    cp->data.assign(data.data(),data.data()+data.size());
    this->sendCommand(cp);
}

