//
// Created by scott2 on 8/11/20.
//

#include "UserAppJsonDataImpl.h"
#include "../message.h"
#include "../app.h"
//#include "../SensorHub.h"
#include "../utils.h"
#include "../GM50/sechannel.h"

void UserAppJsonDataImpl::onCommand(const Command::Ptr& command){
    {
        auto cp = std::dynamic_pointer_cast<CommandUserAppJsonMessage>(command);

        if(!cp) {
            Application::instance()->getLogger().error("unknown message..");
            return ;
        }

        this->channel->setLatestCommand(command);
        return ;

        //

        utils::vector_append<ByteArray>(buff_,cp->data);


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
}

void UserAppJsonDataImpl::parseMessage(const char* data ,size_t size){
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


void UserAppJsonDataImpl::reset(){
    buff_.clear();
}

// 下发设备状态查询  { device_id, params:[ sensor_id, sensor_id,.. ] }
// 转发查询请求给hub
void UserAppJsonDataImpl::onDeviceStatusQuery(const std::shared_ptr<MessageDeviceStatusQuery>& query) {
//    for(auto & p: query->params){
//        std::shared_ptr< MessageSensorStatusQuery> ssq = std::make_shared<MessageSensorStatusQuery>();
//        ssq->sensor_id = p;
//        SensorHub::instance()->sendMessage(ssq);
//    }
}


// 提取并发送  sensor_id 的 控制命令
void UserAppJsonDataImpl::onDeviceValueSet(const std::shared_ptr<MessageDeviceValueSet>& msg){
//    if( msg->params.find("sensor_id") != msg->params.end()) {
//        std::shared_ptr<MessageSensorValueSet> ssq = std::make_shared<MessageSensorValueSet>();
//        ssq->sensor_id = msg->params["sensor_id"];
//        ssq->params = msg->params;
//        SensorHub::instance()->sendMessage(ssq);
//    }
}