
#include "Controller.h"

#include <numeric>
#include <iterator>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <boost/algorithm/string.hpp>


#include "app.h"
#include "utils.h"
#include "version.h"
#include "SensorHub.h"

bool Controller::init(const Config& props){
	boot_time_ = std::time(NULL);
	settings_.login_inited = false;

	cfgs_ = props;
    settings_.comm_server_ip = cfgs_.get_string("server_ip","127.0.0.1");
    settings_.comm_server_port = cfgs_.get_int("server_port",9801);

	resetStatus();
    SensorHub::instance()->init(props);
	check_timer_interval_ = cfgs_.get_int("controller.check_timer",5);
    if( check_timer_interval_ < 30){
//        check_timer_interval_ = 30;
    }

	timer_ = std::make_shared<boost::asio::steady_timer>(io_service_);
	timer_->expires_after(std::chrono::seconds( check_timer_interval_));
	timer_->async_wait(std::bind(&Controller::workTimedTask, this));

    return true;
}

bool Controller::open(){
	net_check_interval_ = cfgs_.get_int("net_check_interval",60);
    SensorHub::instance()->open();
	return true;
}


void Controller::close(){

}

void Controller::run(){
	io_service_.run();
}

Json::Value Controller::getStatusInfo(){

	Json::Value value;
	value["id"] = getDeviceUniqueID();
	value["time"] = (Json::UInt64) std::time(NULL);
	value["host_ver"] = VERSION;
	value["mcu_ver"] = "";
	value["boot_time"] = (Json::UInt64) boot_time_;
	return value;
}

void Controller::resetStatus(){
	std::lock_guard<std::recursive_mutex> lock(rmutex_);
	settings_.login_inited = false;
	if(conn_){
		conn_->close();
		conn_.reset();
	}
}

// 定时工作任务
void Controller::workTimedTask(){
	std::lock_guard<std::recursive_mutex> lock(rmutex_);

//	if(!conn_ && settings_.login_inited){
	if(!conn_ ){
		// 准备发起连接到服务器
		conn_ = std::make_shared<Connection>(get_io_service());
		conn_->setListener( std::dynamic_pointer_cast<IConnectionListener>( shared_from_this()) );
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
		resetStatus();
	}

	if(conn_){
		//发送心跳
		MessageHeartBeat hb;
		hb.device_id = getDeviceUniqueID();
		conn_->send(hb.marshall());
	}

	timer_->expires_after(std::chrono::seconds( check_timer_interval_));
	timer_->async_wait(std::bind(&Controller::workTimedTask, this));

}

std::string Controller::getDeviceUniqueID(){
	// todo. 提供获取设备硬件编码的接口
    std::string uid;
	uid = cfgs_.get_string("id");
    return uid;
}

// 设置设备运行参数值
void Controller::setParameterValues(const PropertyStringMap& params){
	std::string value;
	if(params.find("login_server_url")!= params.end()){
		auto url = boost::lexical_cast<std::string>(params.at("login_server_url"));
		settings_.login_server_url = url;
		saveUserSettings();
	}
	if(params.find("save")!= params.end()){
		saveUserSettings();
	}
	// 设备重启
	if(params.find("reboot")!= params.end()){
		this->reboot();
	}
	// 指定 网卡 dhcp获取ip  nic_dhcp=eth0 令eth0 动态获取
	if(params.find("nic_dhcp")!= params.end()){
		this->reboot();
	}
	// 设置网络接口ip地址
	if(params.find("nic_ip_eth0")!= params.end()){
		auto ip = boost::lexical_cast<std::string>(params.at("nic_ip_eth0"));
		utils::setNicIpAddress("eth0",ip);
	}

	if(params.find("nic_ip_eth1")!= params.end()){
		auto ip = boost::lexical_cast<std::string>(params.at("nic_ip_eth0"));
		utils::setNicIpAddress("eth1",ip);
	}
}

void Controller::setParameterValues(const std::string& name, const std::string&  value, const std::string& module){
    if(name == "login_server_url"){
        auto url = boost::lexical_cast<std::string>(value);
        settings_.login_server_url = url;
        saveUserSettings();
    }
    if( name == "save"){
        saveUserSettings();
    }
    // 设备重启
    if( name == "reboot"){
        this->reboot();
    }
    // 指定 网卡 dhcp获取ip  nic_dhcp=eth0 令eth0 动态获取
    if(  name == "nic_dhcp" ){
        this->reboot();
    }
    // 设置网络接口ip地址
    if( name == "nic_ip_eth0"){
        auto ip = boost::lexical_cast<std::string>( value);
        utils::setNicIpAddress("eth0",ip);
    }

    if( name == "nic_ip_eth1"){
        auto ip = boost::lexical_cast<std::string>( value );
        utils::setNicIpAddress("eth1",ip);
    }
}

void Controller::loadUserSettings(){

}

void Controller::saveUserSettings(){
	Config user;
	user.set_string("login_server_url", settings_.login_server_url);
	user.save(SETTINGS_USER_FILE);
}

void Controller::reboot(){
	::system("reboot");
}


void Controller::onData(const char * data ,size_t size,const Connection::Ptr& conn){
    std::copy_n(data,size,std::back_inserter(buff_));
    auto start = buff_.begin();
    while(true) {
        auto itr = std::find(start,buff_.end(),'\0');
        if(itr != buff_.end()){
            parseMessage( (char*)&*start, std::distance(start,itr));
            itr++;
            start = itr;
        }else{
            break;
        }
    }
    buff_ = ByteArray(start,buff_.end());

}

void Controller::parseMessage(const char* data ,size_t size){
    std::string msg(data,size);
    Application::instance()->getLogger().warn(msg);

    Message::Ptr message = MessageJsonParser::parse(data,size);
    if(!message){
        Application::instance()->getLogger().error("MessageJsonParser::parse Error..");
        return ;
    }
    Application::instance()->getLogger().debug("onJsonText : "+ message->name());
    { // 设备运行状态查询
        std::shared_ptr<MessageDeviceStatusQuery> msg = std::dynamic_pointer_cast<MessageDeviceStatusQuery>(message);
        if(msg) {
            this->onDeviceStatusQuery(msg);
            return;
        }
    }

    { //设备参数设置
        std::shared_ptr<MessageDeviceValueSet> msg = std::dynamic_pointer_cast<MessageDeviceValueSet>(message);
        if(msg) {
            this->onDeviceValueSet(msg);
            return;
        }
    }

    { //心跳
        std::shared_ptr<MessageHeartBeat> msg = std::dynamic_pointer_cast<MessageHeartBeat>(message);
        if(msg) {
//                last_heart_time_ = std::time(NULL);
            return;
        }
    }
}


void Controller::onConnected(const Connection::Ptr& conn){
	std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    buff_.clear();

	std::shared_ptr<MessageLogin> login_req = std::make_shared<MessageLogin> ();
	login_req->device_id = getDeviceUniqueID();
	login_req->token = settings_.token;
	// 呼叫连接建立，发送登录消息包
	conn->send( login_req->marshall());
}

//外部连接丢失
void Controller::onDisconnected(const Connection::Ptr& conn){
	std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
	Application::instance()->getLogger().debug("Connection Disconnected..");
	resetStatus();

//	live_time_inside_ = live_time_outside_ = 0;
}

void Controller::onData( boost::asio::streambuf& buffer,const Connection::Ptr& conn){

}

//对外连接失败
void Controller::onConnectError(const Connection::Ptr& conn,ConnectionError error){
	std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
	resetStatus();

}

void Controller::onJsonText(const std::string & text,const Connection::Ptr& conn){
	std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    Application::instance()->getLogger().debug("<< Data From Remote Server : " + text);
	Message::Ptr message = MessageJsonParser::parse(text.c_str(),text.size());
	if(!message){
		return ;
	}
	std::shared_ptr<MessagePayload> payload;
	Application::instance()->getLogger().debug("onJsonText : "+ message->name());
	{
		std::shared_ptr<MessageLoginResp> msg = std::dynamic_pointer_cast<MessageLoginResp>(message);
		if (msg) {
			// 登录反馈
			if(msg->error){
				Application::instance()->getLogger().error("login error:" + boost::lexical_cast<std::string>(msg->error));
				resetStatus();
			}
			return;
		}
	}

	{ // 设备运行状态查询
		std::shared_ptr<MessageDeviceStatusQuery> msg = std::dynamic_pointer_cast<MessageDeviceStatusQuery>(message);
		if(msg) {
            this->onDeviceStatusQuery(msg);
			return;
		}
	}

	{ //设备参数设置
		std::shared_ptr<MessageDeviceValueSet> msg = std::dynamic_pointer_cast<MessageDeviceValueSet>(message);
		if(msg) {
            this->onDeviceValueSet(msg);
			return;
		}
	}

	{ //心跳
		std::shared_ptr<MessageHeartBeat> msg = std::dynamic_pointer_cast<MessageHeartBeat>(message);
		if(msg) {
			last_heart_time_ = std::time(NULL);
			return;
		}
	}
}

// 发送到云端
void Controller::onTraverseUpMessage(const std::shared_ptr<MessageTraverseUp> message){
    Application::instance()->getLogger().debug("onTraverseUpMessage :" + message->marshall());

	std::lock_guard<std::recursive_mutex> lock(rmutex_);
	message->device_id = getDeviceUniqueID();
	if(conn_){
		conn_->send(message->marshall());
	}
}

void Controller::onTraverseDownMessage(const std::shared_ptr<MessageTraverseDown> message){

}

// 下发设备状态查询  { device_id, params:[ sensor_id, sensor_id,.. ] }
// 转发查询请求给hub
void Controller::onDeviceStatusQuery(const std::shared_ptr<MessageDeviceStatusQuery>& query) {
    for(auto & p: query->params){
        std::shared_ptr< MessageSensorStatusQuery> ssq = std::make_shared<MessageSensorStatusQuery>();
        ssq->sensor_id = p;
        SensorHub::instance()->sendMessage(ssq);
    }
}


// 提取并发送  sensor_id 的 控制命令
void Controller::onDeviceValueSet(const std::shared_ptr<MessageDeviceValueSet>& msg){
    if( msg->params.find("sensor_id") != msg->params.end()) {
        std::shared_ptr<MessageSensorValueSet> ssq = std::make_shared<MessageSensorValueSet>();
        ssq->sensor_id = msg->params["sensor_id"];
        ssq->params = msg->params;
        SensorHub::instance()->sendMessage(ssq);
    }
}

