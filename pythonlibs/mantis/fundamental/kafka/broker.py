#coding:utf-8


"""
"""

import traceback
from threading import Thread,Condition,Lock
from pykafka import  KafkaClient
from pykafka.common import OffsetType
from mantis.fundamental.utils.importutils import import_function


class MessageChannel(object):
    def __init__(self,cfgs,broker):
        self.cfgs = cfgs
        self.broker = broker
        self.message_handler = None

        self.name = cfgs.get('name')
        self.group = cfgs.get('group', '')
        self.hosts = broker.cfgs.get('hosts')
        self.zookeepers = broker.cfgs.get('zookeepers')

        handler = cfgs.get('handler')
        if handler:
            self.message_handler = import_function(handler)  # importing functions dynamically

        # self.access_cfg = filter(lambda x: len(x) > 0, cfg.get('access', '').strip().upper().split(','))

        self.conn = None
        self.topic = None
        self.producer = None
        self.consumer = None
        self.cond_readable = Condition()
        self.thread = None
        self.isclosed = True

    def open(self):
        from mantis.fundamental.application.use_gevent import USE_GEVENT
        if self.conn is None:
            self.conn = KafkaClient(hosts=self.hosts)
            self.topic = self.conn.topics[self.name]

        if self.message_handler:
            self.subscribe('',self.message_handler)

    def close(self):
        self.isclosed = True
        if self.consumer:
            self.consumer.stop()
        self.join()


    def join(self):
        if self.thread:
            self.thread.join()

    def produce(self,message):

        if  self.producer is None:
            self.producer = self.topic.get_producer(delivery_reports=False)
        self.producer.produce(message)

    def _new_consumer(self):
        if not self.consumer:
            if self.group:
                self.consumer = self.topic.get_balanced_consumer(consumer_group=self.group,
                                                                 auto_commit_enable=True,
                                                                 reset_offset_on_start=True,
                                                                 auto_offset_reset=OffsetType.LATEST,
                                                                 zookeeper_connect=self.zookeepers
                                                                 )
            else:
                self.consumer = self.topic.get_simple_consumer(auto_commit_enable=True,
                                                               reset_offset_on_start=True,
                                                               auto_offset_reset=OffsetType.LATEST
                                                               )

    def consume(self,block=True):
        self._new_consumer()
        msg = self.consumer.consume(block)
        if msg:
            self.consumer.commit_offsets()
        return msg

    def subscribe(self,pattern, message_handler=None):
        self.message_handler = message_handler
        if self.message_handler:
            self.thread = Thread(target=self.message_recieving)
            self.thread.start()

    def unsubscribe(self,pattern):
        pass


    def publish(self,message):
        self.produce(message)

    def message_recieving(self):
        self.isclosed = False

        self._new_consumer()

        while not self.isclosed:
            for message in self.consumer:
                if message is not None:
                    # print message.offset, message.value
                    if self.consumer._consumer_group:
                        self.consumer.commit_offsets()
                    ctx ={
                        "channel":self,
                        "message":message
                    }
                    try:
                        self.message_handler(message.value, ctx)
                    except:
                        print message
                        traceback.print_exc()



class MessageBroker(object):
    def __init__(self):
        self.cfgs = {}
        self.channels = {}

    def init(self,cfgs):
        self.cfgs = cfgs
        for cfg in self.cfgs.get('channels',[]):
            if cfg.get('enable', False) is False:
                continue
            self.channels[cfg.get('name')] = MessageChannel(cfg,self)

    def open(self):
        for channel in self.channels.values():
            channel.open()

    def close(self):
        for channel in self.channels:
            channel.close()

    def join(self):
        pass

    def get(self, name):
        return self.channels.get(name)