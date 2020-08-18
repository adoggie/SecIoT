//
// Created by scott on 2020-07-04.
//

#include "sensor.h"

std::shared_ptr<MessageSensor> MessageSensorStatus::parse(const Json::Value& root){
    std::shared_ptr<MessageSensor> result;
    if( root["message"].asString() ==  MESSAGE_STATUS){
        std::shared_ptr<MessageSensorStatus> msg = std::make_shared<MessageSensorStatus>();
        msg->sensor_id = root["sensor_id"].asString();
        auto node = root["params"];
        if(node.isObject()){
            auto names = node.getMemberNames();
            for(auto & key : names){
                auto value = node[key].asString();
                msg->params[key] = value;
            }
        }
        result = msg ;
    }
    return result;
}