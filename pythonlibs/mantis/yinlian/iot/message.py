#coding:utf-8

from mantis.fundamental.utils.useful import hash_object,object_assign

from mantis.fundamental.network.message import JsonMessage
from mantis.yinlian.iot.base import *


class MessageTraverse(JsonMessage):
    OFFSET_BID = 0  #
    OFFSET_DOWN = 1
    OFFSET_UP = 2
    NAME = ''

    def __init__(self, name):
        JsonMessage.__init__(self, name)
        self.device_id = ''

    def values(self):
        return hash_object(self,excludes=('id_','name_','values_','extras_','NAME','OFFSET_BID','OFFSET_DOWN','OFFSET_UP'))

class MessageTraverseDown(MessageTraverse):
    """下行消息"""

    def __init__(self, name):
        MessageTraverse.__init__(self, name)
        # self.offset = self.OFFSET_DOWN

class MessageTraverseUp(MessageTraverse):
    """上行消息"""

    def __init__(self, name):
        MessageTraverse.__init__(self, name)

class MessageLogin(MessageTraverseUp):
    """设备登陆请求"""
    NAME = 'login'

    def __init__(self):
        MessageTraverseUp.__init__(self, self.NAME)
        self.token = ''

class MessageLoginResp(MessageTraverseDown):
    """设备登陆反馈消息"""
    NAME = 'login_resp'

    def __init__(self):
        MessageTraverseDown.__init__(self, self.NAME)
        self.error = 0  # 错误码 0 : 成功
        self.message = ''
        self.server_time = 0


class MessageHeartBeat(MessageTraverse):
    """设备与平台之间的心跳消息"""
    NAME = 'heartbeat'

    def __init__(self):
        MessageTraverse.__init__(self, self.NAME)


class MessageDeviceStatusQuery(MessageTraverseDown):
    """平台下发设备状态查询请求"""
    NAME = 'data_query'

    def __init__(self):
        MessageTraverseDown.__init__(self, self.NAME)
        self.params = []    # 指定查询的参数列表，缺省表示返回所有设备状态值

class MessageDeviceStatus(MessageTraverseUp):
    NAME = 'data_report'
    def __init__(self):
        MessageTraverseUp.__init__(self, self.NAME)
        self.type = 'status'
        self.params = {}  # 指定特定功能的运行参数



class MessageDeviceValueSet(MessageTraverseDown):
    """设备运行参数设置"""
    NAME = 'data_set'

    def __init__(self):
        MessageTraverseDown.__init__(self, self.NAME)
        self.params = {}

#
# class MessageSensorStatusQuery(MessageTraverseUp):
#     """查询指定传感器模块运行参数"""
#     NAME = 'sensor_status_query'
#
#     def __init__(self):
#         MessageTraverseUp.__init__(self, self.NAME)
#         self.sensor_type = SensorType.All
#         self.sensor_id = Constants.Undefined
#
#
# class MessageSensorStatus(MessageTraverseUp):
#     """上传传感器模块运行参数"""
#     NAME = 'sensor_status'
#
#     def __init__(self):
#         MessageTraverseUp.__init__(self, self.NAME)
#         self.sensor_type = SensorType.Undefined
#         self.sensor_id = Constants.Undefined
#         self.params = {}  # 指定特定功能的运行参数
#
#
# class MessageSensorValueSet(MessageTraverseDown):
#     """下发对传感器的控制"""
#     NAME = 'sensor_val_set'
#
#     def __init__(self):
#         MessageTraverseDown.__init__(self, self.NAME)
#         self.sensor_type = SensorType.Undefined
#         self.sensor_id = Constants.Undefined
#         self.param_name = ''
#         self.param_value = ''

MessageClsDict ={}

def registerMessageObject(msgcls):
    MessageClsDict[msgcls.NAME] = msgcls


for key,value in locals().items():
    if key.find('Message')==0 and key not in ('MessageClsDict','Message','MessageType','MessageSplitter'):
        registerMessageObject(value)

def parseMessage(data):
    print data
    if  isinstance(data,str):
        data = json.loads(data)

    message = data.get('name')
    msgcls = MessageClsDict.get(message)
    if not msgcls:
        print 'Message Type unKnown. value:{}'.format(message)
        return None
    data = data.get('values',{})
    msg = msgcls()
    msg.assign(data)
    return msg

if __name__=='__main__':
    data='''{
        "id": "",
        "name": "sensor_status",
        "values": {
            "params": {
                "1": "0"
            },
            "sensor_id": 1,
            "sensor_type": 1
        }
    }'''
    m = parseMessage(data)
    print m.id
