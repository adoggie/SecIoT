
GWServer
========
控制主机的云端通信接入服务器

## 主体功能

1. 设备的上线、下线、消息进行推送到前端客户(socket.io)
2. 设备的控制命令发送方式： 即时发送（设备在线）、延时发送（设备离线）

## 环境&开发

    python
    gevent 
    mongodb
    redis 

## 过程

1. settings.yaml 的 redis/datasource 配置接收来自 iot的消息队列  pubsub (iot_data_chan/handlers.iot.get_message)
2. 外部系统通过http的接口接收控制指令并下发到线下设备端


## Requirements

pip install protobuf 


## kafka

```bash
bin/kafka-topics.sh --create --zookeeper server1:2181 --replication-factor 1 --partitions 1 --topic dtu_topic
bin/kafka-topics.sh --describe --zookeeper server1:2181 --topic dtu_topic
bin/kafka-console-producer.sh --broker-list server1:9092 --topic dtu_topic
bin/kafka-console-consumer.sh --bootstrap-server server1:9092 --from-beginning --topic dtu_topic

```

./bin/kafka-topics.sh --bootstrap-server server1:9092 --topic dtu_topic --delete

./bin/kafka-delete-records.sh --bootstrap-server server1:9092 --offset-json-file ./del.json 

del.json 

{
  "partitions": [
    {
      "topic": "dtu_topic",
      "partition": 0,
      "offset": 0
    }
  ],
  "version": 1
}


