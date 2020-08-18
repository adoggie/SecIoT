# coding:utf-8

"""
dtu 主机接入服务器

"""

import sys
from optparse import OptionParser

from mantis.fundamental.utils.useful import singleton
from mantis.fundamental.application.app import instance
from mantis.fundamental.network.socketutils import Server
from mantis.fundamental.application.service import BaseService
from mantis.fundamental.utils.importutils import import_class

from mantis.yinlian.iot import model
from mantis.yinlian.iot.message import  *
from mantis.yinlian.iot.model import set_database


class MainService(BaseService):
    def __init__(self,name):
        BaseService.__init__(self,name)
        self.logger = instance.getLogger()
        self.servers = {}
        self.adapters ={}
        self.apps = {}      # app 软件连接进入

    def init(self, kwargs):
        BaseService.init(self,**kwargs)
        # self.init_database()

        for svrcfg in self.cfgs.get('servers',[]):
            if not svrcfg.get('enable',False):
                self.logger.info("server:{} skipped..".format(svrcfg.get('name')))
                continue
            cls = import_class(svrcfg.get('handler_cls',{}).get('class'))

            svrcfg['handler_cls_kwargs'] = svrcfg.get('handler_cls',{}).get('kwargs',{})
            svrcfg['handler_cls'] = cls
            server = Server().init(**svrcfg)
            self.servers[server.name] = server

    def init_database(self):
        conn = instance.datasourceManager.get('mongodb').conn
        db = conn['SmartHome']
        set_database(db)
        return db

    def setupFanoutAndLogHandler(self):
        self.initFanoutSwitchers(self.cfgs.get('fanout'))

    def start(self,block=True):
        BaseService.start(self)
        for server in self.servers.values():
            server.start()

    def stop(self):
        BaseService.stop(self)

    def initCommandChannels(self):
        #BaseService.initCommandChannels(self)
        pass

    def getActivedDevices(self):
        return self.adapters.values()

    def deviceOnline(self, adapter):
        """设备上线"""
        self.adapters[adapter.device_id] = adapter

    def deviceOffline(self,adapter):
        """设备离线"""
        for device_id,item in self.adapters.items():
            if item == adapter:
                del self.adapters[device_id]
                break


