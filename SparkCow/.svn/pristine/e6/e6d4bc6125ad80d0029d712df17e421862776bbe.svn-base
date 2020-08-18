//
// Created by scott on 2020-07-04.
//

#include "SensorHub.h"
#include "Controller.h"
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>

#include <boost/algorithm/string.hpp>

typedef std::function< std::shared_ptr<MessageSensor> (const Json::Value& root) > ParseFuncSensor;


std::vector<ParseFuncSensor> parse_func_list_sensor={
        MessageSensorStatus::parse
};


std::shared_ptr< MessageSensor > SensorHub::parse(const char * data , size_t size){
    Json::Reader reader;
    Json::Value root;
    std::shared_ptr< MessageSensor > msg;
    if (reader.parse(data, root)){
        for(auto func:parse_func_list_sensor){
            msg = func(root);
            if(msg){
                break;
            }
        }
    }
    return msg;

}

bool SensorHub::init(const Config & cfgs){
    // init nonamsg
    // init pub/sub

    auto pub_addr = cfgs.get_string("sensor_pub_address");
    auto sub_addr = cfgs.get_string("sensor_sub_address");

//    auto text = cfgs.get_string("sensor_id_list");
//    std::vector<std::string> values;
//    boost::split( values,text,boost::is_any_of(","));
//    auto subtopic = cfgs.get_string("sensor_sub_topic");
//    topic_sub_ =  subtopic;

    int sock = nn_socket (AF_SP, NN_PUB);
    if (sock == -1) {
        return false;
    }
    int rc;
    rc = nn_bind (sock, pub_addr.c_str());
    if( rc < 0){
        return false;
    }
    this->sensor_pub_ = sock;

    this->sensor_sub_ = nn_socket (AF_SP, NN_SUB);
//    rc = nn_setsockopt (this->sensor_sub_, NN_SUB, NN_SUB_SUBSCRIBE, subtopic.c_str(), subtopic.size());

    rc = nn_bind (this->sensor_sub_, sub_addr.c_str());
    if( rc < 0){
        return false;
    }

    rc = nn_setsockopt (this->sensor_sub_, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
    if( rc < 0){
        std::cout<< "Error: sub failed." << std::endl;
        return false;
    }
//    rc = nn_connect (this->sensor_sub_, address.c_str());
//    if( rc<0 ){
//        std::cout<< "Error: sub connect failed." << std::endl;
//        return false;
//    }


    return true;
}

void SensorHub::recvSubData(){
    size_t data_len;
    int rc;
//    char buf[1024];
    running_ = true;
    char *buf;

    std::cout<< " Thread recvSubData started." << std::endl;
    while( running_ ){
//        memset(buf,0,sizeof buf);
//        rc = nn_recv (this->sensor_sub_, buf, sizeof buf, 0);
        rc = nn_recv (this->sensor_sub_, &buf, NN_MSG, 0);
        if (rc < 0) {
            std::cout<< "Error: recvSubData error." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }
        std::string text(buf,rc);
        std::cout<< "<< MX:" << buf << std::endl;

        nn_freemsg(buf);
        std::shared_ptr<MessageSensor>  message = this->parse(buf,rc);
        this->onMessage(message);

    }
    std::cout<< " Thread recvSubData end." << std::endl;
}

bool SensorHub::open(){
//    std::thread  thread();
    work_thread_ = std::make_shared<std::thread>(&SensorHub::recvSubData,this);
    return true;
}

void SensorHub::close(){
    this->running_ = false;
}

void SensorHub::sendMessage(const std::shared_ptr<MessageSensor> & message){
    // 根绝 message.sensor_id  将消息publish过去
    auto data = message->values().toStyledString();
    auto topic = message->sensor_id;
    auto msgtext = topic + data;
    int rc;
    rc = nn_send (this->sensor_pub_, msgtext.c_str(), msgtext.size(), 0);
    if (rc < msgtext.size()){
        std::cout<<" Error: pub message error." << std::endl;
    }
}

void SensorHub::onMessage( std::shared_ptr<MessageSensor> & message){

    { //

        std::shared_ptr<MessageSensorStatus> status = std::dynamic_pointer_cast<MessageSensorStatus>(message);
        if (status) {
            std::shared_ptr<MessageDeviceStatus> up = std::make_shared<MessageDeviceStatus>();
            up->params = std::move( status->params);
            up->params["sensor_id"] = status->sensor_id;
            Controller::instance()->onTraverseUpMessage(up);
        }

    }

}

