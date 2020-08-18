//
// Created by scott on 2020-07-04.
//

#ifndef SMARTBOX_SENSOR_H
#define SMARTBOX_SENSOR_H

#include "message.h"

struct MessageSensor{
    std::string message;
    std::string sensor_id;
    MessageSensor(const std::string & message_):message(message_){

    }
    virtual ~MessageSensor(){};
    virtual Json::Value values(){ return Json::Value();};
};

//请求 sensor 上报状态  down
struct MessageSensorStatusQuery:MessageSensor{
    MessageSensorStatusQuery():MessageSensor(MESSAGE_STATUS_QUERY){
    }

    Json::Value values(){
        Json::Value root;
        root["message"] = message;
        root["sensor_id"] = sensor_id;
        return root;
    }

};


struct MessageSensorStatus:MessageSensor{
    PropertyStringMap params;
    MessageSensorStatus():MessageSensor(MESSAGE_STATUS){

    }


    static std::shared_ptr<MessageSensor> parse(const Json::Value& root);
};

struct MessageSensorValueSet:MessageSensor{
    PropertyStringMap params;

    MessageSensorValueSet():MessageSensor(MESSAGE_VALUE_SET){
    }

    Json::Value values(){
        Json::Value root;
        root["message"] = message;
        root["sensor_id"] = sensor_id;
        Json::Value array;
        for(auto & _ : params){
            array[_.first] = _.second;
        }
        root["params"] = array;
        return root;
    }

};



#endif //SMARTBOX_SENSOR_H
