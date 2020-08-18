//
// Created by scott on 2020-07-04.
//

#ifndef SMARTBOX_SENSORHUB_H
#define SMARTBOX_SENSORHUB_H

#include "sensor.h"
#include "config.h"
// 传感器集线器

class SensorHub {
public:
    typedef std::shared_ptr<SensorHub> Ptr;
    static std::shared_ptr<SensorHub>& instance(){
        static std::shared_ptr<SensorHub> handle ;
        if(!handle){
            handle = std::make_shared<SensorHub>() ;
        }
        return handle;
    }

    bool init(const Config & cfgs);
    bool open();
    void close();

    ///分派下行消息到Sensor设备
    void sendMessage(const std::shared_ptr<MessageSensor> & message);
protected:
    std::shared_ptr< MessageSensor > parse(const char * data , size_t size);
    void onMessage( std::shared_ptr<MessageSensor> & message);

    void recvSubData();
private:
//    std::map<std::string, int> sensor_pub_; // 多个sensor的接收地址
    std::string topic_sub_;
    int sensor_pub_;
    int sensor_sub_; // 接收所有sensor上报的消息
    std::atomic_bool running_;
    std::shared_ptr<std::thread> work_thread_;
};


#endif //SMARTBOX_SENSORHUB_H
