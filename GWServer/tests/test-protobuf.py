
from mantis.yinlian.iot.tb.transport_pb2 import TransportToDeviceActorMsg

# protoc -I. --python_out=./ transport.proto
# reference:
# https://developers.google.com/protocol-buffers/docs/reference/python-generated#repeated-message-fields


# To Encode Message
msg = TransportToDeviceActorMsg()
msg.sessionInfo.nodeId = "001"
msg.sessionInfo.sessionIdMSB = 110
msg.sessionInfo.tenantIdMSB = 110

kv = msg.postAttributes.kv.add()
kv.key = "name"
kv.string_v = "scott"

kv = msg.postAttributes.kv.add()
kv.key = "age"
kv.string_v = "10"

s = msg.SerializeToString()
print len(msg.postAttributes.kv)
for attr in msg.postAttributes.kv:
  print attr.key,attr.string_v
print msg
print len(s)

# To Decode Message
x = TransportToDeviceActorMsg()
data = x.ParseFromString(s)
print x
print data

