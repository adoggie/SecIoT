#coding:utf-8

import json
import time, os,os.path,datetime
import traceback
import gevent
from gevent.queue import Queue

from mantis.fundamental.application.app import instance
from mantis.fundamental.utils.timeutils import timestamp_current,current_datetime_string
from mantis.fundamental.utils.timeutils import str_to_timestamp,timestamp_current
from mantis.fundamental.network.socketutils import ConnectionEventHandler
from mantis.fundamental.network.accumulator import JsonDataAccumulator

from mantis.yinlian.iot.message import *
from mantis.yinlian.iot import constants
from mantis.yinlian.iot import model
from mantis.yinlian.iot import mx


class SmartAdapter(ConnectionEventHandler):
    def __init__(self):
        ConnectionEventHandler.__init__(self)
        self.cfgs = {}
        self.accumulator = JsonDataAccumulator()

        self.conn = None
        self.session = None

        self.service = instance.serviceManager.get('main')
        # self.mongo = instance.datasourceManager.get('mongodb').conn
        # self.redis = instance.datasourceManager.get('redis').conn

        self.raw_file = None
        self.active = False  # 设备是否在线登录
        self.start_time = datetime.datetime.now()
        self.device_id = None
        self.redis = None
        self.device = None
        self.logger = None

        # self.packet_sequence = 0

        # self.device_iot_channel = None  # 发送到绿城家的通道
        self.iot_controller = None

        self.device_app_pub_channel = None
        self.command_controller = None
        self.queue = Queue()
        self.running = True
        self.peer_address = ''
        self.last_heartbeat = 0
        # self.msg_codec = MessageJsonStreamCodec()
        self.box = None
        self.device_type = ''
        self.channel_down = None

    def init(self,**kwargs):
        self.cfgs.update(kwargs)
        # self.redis = instance.datasourceManager.get('redis').conn
        self.logger = instance.getLogger()

        # cls = import_class(self.cfgs.get('command_controller'))
        # self.command_controller = cls()

        self.broker = instance.messageBrokerManager.get('redis')

        gevent.spawn(self.messageProcessTask)

    def onConnected(self,conn,address):
        """连接上来"""
        self.logger.debug('device connected .  {}'.format(str(address)))
        self.conn = conn
        self.peer_address = address  # 连接上来的对方地址
        self.last_heartbeat = timestamp_current()

    def onDisconnected(self,conn):
        self.logger.debug('device  disconnected. {} {}'.format(self.device_type, self.device_id))
        self.active = False
        # self.raw_file.close()
        self.service.deviceOffline(self)
        self.running = False

        # 断开连接删除设备与接入服务器的映射
        # self.redis.delete(constants.DeviceServerRel.format(self.device_id))

        # if self.iot_controller:
        #     self.iot_controller.onDeviceDisconnected()
        if self.channel_down:
            self.channel_down.close()



    def onData(self,conn,data):
        self.logger.debug("<< Device Data Retrieve in . {} {}".format(self.device_id, self.device_type))
        self.logger.debug(data)
        json_text_list = self.accumulator.enqueue(data)

        # dump = self.hex_dump(bytes)
        # self.logger.debug("<< "+dump)
        # self.dump_hex_data(dump)

        for text in json_text_list:
            message = parseMessage(text)
            if message:
                self.queue.put([message, current_datetime_string()])

    def checkLogin(self,message):
        # from mantis.yinlian.dtu.token import device_token_check
        self.device_id = message.device_id
        return True

        # device = model.SmartDevice.get(id = message.device_id)
        # if not device :
        #     return False
        # secret = device.secret_key
        # data = device_token_check(message.token,secret)
        # if not data:
        #     return False
        # auth_time = data.get('auth_time',0)
        # self.device_id = data.get('id')
        # self.device_type = data.get('type')
        # TOKEN_VALID_TIME = self.service.getConfig().get('token_valid_time',300)
        # # 校验token是否已过期
        # if timestamp_current() - auth_time > TOKEN_VALID_TIME:
        #     instance.getLogger().error(u'过期无效token: ',message.token)
        #     return False
        #
        # return True

    def handleMessage(self,message):
        """ 处理来自设备上行的消息
        """
        # self.logger.debug( str( message.__class__ ))
        self.logger.debug('{} conn:{}'.format(str(message),str(self.conn)))
        # self.logger.debug(message.__class__.__name__+' device_type:{} device_id:{} message type:0x{:02x} '.format(self.device_type,self.device_id,message.Type.value))
        # self.logger.debug(
        #     message.__class__.__name__ + ' device_type:{} device_id:{} content: {} '.format(self.device_type,
        #                                                                                               self.device_id,
        #                                                                                               message.dict()))
        if isinstance(message,MessageHeartBeat):
            return self.handleHeartbeat(message)

        if 0 and  not self.active: # 设备未登陆
            if isinstance(message, MessageLogin): # 设备注册包
                if not self.checkLogin( message ):
                    self.logger.error('Login Info Check Failed.')
                    self.close()
                    return
                self.onActive() # 设备登陆注册
            else:
                self.logger.error('Device should Login first, socket connection dropped')
                self.close()
                return

        if not message.device_id:
            message.device_id = self.device_id
        # message.device_type = self.device_type

        # self.postMessageIoT(message)  # 将设备信息分发到 绿城+SDK --> iot

        # 发布到app接收通道
        device_id = message.device_id
        # name = constants.DeviceChannelPubTraverseUp.format(device_id=device_id)
        # 推入 本地 redis
        # self.broker.conn.publish(name, message.marshall(''))



        # if isinstance(message,MessageHeartBeat):
        #     self.handleHeartbeat(message)

        if isinstance(message,MessageDeviceStatus):
            self.handleDeviceStatus(message)

        # 推入平台消息总线
        if isinstance(message,MessageDeviceStatus):
            mx.put_message(message)

    def handleDeviceStatus(self,message):
        pass



    def handleHeartbeat(self,message):
        self.last_heartbeat = timestamp_current()
        if self.device:
            self.device.alive_time_s = datetime.datetime.now()
            self.device.alive_time = timestamp_current()
            self.device.save()


    def open(self):
        pass

    def setConnection(self,sock_conn):
        self.conn = sock_conn

    def close(self):
        self.running = False
        if self.conn:
            self.conn.close()


    def make_rawfile(self):
        fmt = '%Y%m%d_%H%M%S.%f'
        ts = time.time()
        name = time.strftime(fmt, time.localtime(ts)) + '.raw'
        name = os.path.join(instance.getDataPath(),name)
        self.raw_file = open(name,'w')


    def hex_dump(self,bytes):
        dump = ' '.join(map(lambda _:'%02x'%_, map(ord, bytes)))
        return dump


    def threadHeartbeat(self):
        """定时发送设备的心跳包"""
        sleep = self.cfgs.get('heartbeat', 10)
        while self.running:
            gevent.sleep( sleep )
            if not self.running:
                break
            self.traverseDown(MessageHeartBeat())

    def onTraverseMessage(self,data,ctx):
        """接收到平台服务发送的下行的消息，转发到下行的设备连接对端"""
        message = parseMessage(data)
        self.traverseDown(message)

    def subscribeTraverseDownMessage(self):
        """订阅下行控制命令"""
        broker = instance.messageBrokerManager.get('redis')
        name = constants.DeviceChannelPubTraverseDown.format(device_id = self.device_id)

        self.channel_down = broker.createPubsubChannel(name, self.onTraverseMessage)
        self.channel_down.open()

    def onActive(self):
        """设备在线登录, 订阅本设备下行的消息"""
        self.logger.debug('device onActive. {} {}'.format(self.device_type, self.device_id))
        self.active = True
        gevent.spawn(self.threadHeartbeat)  # 定时心跳发送

        # access_url = self.service.getConfig().get('access_api_url')     #暴露给外部调用的web接口，接收命令控制
        # self.redis.set( constants.DeviceAccessHttpAPI.format(self.device_id),access_url)
        #
        # device = model.SmartDevice.get(id = self.device_id)
        # if device:
        #     self.device = device
        # else:
        #     self.logger.error('device not register',self.device_id)
        #     self.close()
        #     return
        #
        # CHECK_ACTIVE = False
        # if CHECK_ACTIVE:
        #     if not device.active_time:  # 未激活
        #         self.logger.error('device not actived.',self.device_id)
        #         self.close()
        #         return
        #
        # device.alive_time = timestamp_current()
        # device.save()

        self.service.deviceOnline(self)
        self.subscribeTraverseDownMessage()

        # 设备上线，即刻发送设备状态查询
        self.traverseDown(MessageDeviceStatusQuery())

        # todo. 发送设备上线 事件 到平台总线


    def traverseDown(self,message):
        """转发设备下行消息"""
        self.conn.sendData(message.marshall())

        if isinstance(message, MessageDeviceValueSet):
            pass
            # log = model.LogDeviceValueSet()
            # log.device_id = self.device_id
            # log.sys_time = timestamp_current()
            # log.param_name = message.param_name
            # log.param_value = message.param_value
            # log.save()

    def messageProcessTask(self):
        """处理设备上报消息的工作队列"""
        while self.running:
            # 长久没有心跳包
            if timestamp_current() - self.last_heartbeat > 60*5:
                self.logger.warn('Heartbeat Lost. Close socket. {} {} '.format(self.device_id, self.device_type))
                self.close()
                break
            try:
                message,date = self.queue.get(block=True, timeout=1)
                try:
                    self.logger.debug('message pop from queue: {} {}  {} {}'.format(date,message.__class__.__name__,self.device_id,self.device_type))
                    self.handleMessage(message)
                except:
                    self.logger.warn(traceback.print_exc())
                    self.close()
                    break
            except:pass

        # todo. 此处发送设备 断开的事件 到平台总线
        self.logger.warn('messageTask() exiting.. {} {} '.format(self.device_id, self.device_type))




