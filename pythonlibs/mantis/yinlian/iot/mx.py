#coding:utf-8

# mx.py
# 消息总线的上上下下的享受

from mantis.fundamental.application.app import instance
from mantis.yinlian.iot.tb.transport_pb2 import TransportToDeviceActorMsg,ToRuleEngineMsg,KeyValueType,STRING_V
from mantis.yinlian.iot.tb.transport_pb2 import ToTransportMsg,DeviceActorToTransportMsg
from mantis.yinlian.iot.message import MessageDeviceValueSet,MessageDeviceStatusQuery
from mantis.yinlian.iot.constants import DeviceChannelPubTraverseDown

# protoc -I. --python_out=./ transport.proto

def put_message(message):
  """
  上报 dtu 设备消息到 iot 平台总线
  message:
    {
      device_id: xxx,
      params:{
        sensor_id: xxx,
        others: xxx
        ...
      }
    }
    to see: mantis.yinlian.iot.message.MessageDeviceStatus
  """
  # topic = instance.messageBrokerManager.get('kafka').channels.values()[0]
  topic = instance.messageBrokerManager.get('kafka').channels['dtu_topic']

  # To Encode Message
  remsg = ToRuleEngineMsg()
  # remsg.toDeviceActorMsg =TransportToDeviceActorMsg()
  msg = remsg.toDeviceActorMsg

  kv = msg.postAttributes.kv.add()
  kv.key = "device_id"
  kv.string_v = message.device_id
  kv.type = STRING_V
  for k,v in message.params.items():
    kv = msg.postAttributes.kv.add()
    kv.key = k
    kv.string_v = v
    kv.type = STRING_V

  msg.sessionInfo.nodeId = message.device_id
  msg.sessionInfo.deviceIdLSB = 0x1122334455667788
  msg.sessionInfo.deviceIdMSB = 0x0099aabbccddeeff

  data = remsg.SerializeToString()
  topic.publish( data )

  print 'Message be Pushed into MX..', len(data) , ' Bytes.'

def get_message(message,ctx):
  """ recv message from kafka
    提取 iot 平台总线发送的下行设备控制指令，转发到 gwserver读取的消息总线 (redis)

    See: mantis.yinlian.iot.message.MessageDeviceValueSet

  """
  # message is  <ToTransportMsg>

  try:
    tm = ToRuleEngineMsg.ParseFromString(message)
    m = tm.toDeviceActorMsg


    tm = ToTransportMsg.ParseFromString(message)
    # tm.DeviceActorToTransportMsg
    #
    downmsg = MessageDeviceValueSet()
    device_id = message.device_id
    name = DeviceChannelPubTraverseDown.format(device_id=device_id)
    # 推入 本地 redis
    broker = instance.messageBrokerManager.get('redis')
    broker.conn.publish(name, message.marshall(''))
  except:
    print 'Corrupted Message recved from MX..' , len(message) , ' Bytes'


"""
kafka message publish test:
--------------

bin/kafka-console-producer.sh --broker-list localhost:9092 --topic tes-topic

"""