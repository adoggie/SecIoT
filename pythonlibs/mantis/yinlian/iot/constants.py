#coding:utf-8


DeviceCommandQueue = 'iot.device.command.queue.{device_type}.{device_id}'

DeviceChannelPub = 'iot.device.channel.pub.{device_id}' # 设备所有原始数据读取之后分发的通道

DeviceAppChannelPub = 'iot.device.app.channel.pub.{device_id}'  # 设备监控应用通道，所有的前端系统将订阅此通道将监控信息推送到前端App

DeviceChannelPubIoT = '{device_id}'  # 推送到绿城+的发布通道

DeviceChannelPubTraverseDown = 'iot.down.pub.{device_id}'  # 设备下发控制命令的通道
DeviceChannelPubTraverseUp = 'iot.up.pub.{device_id}'      # 设备上行消息分发通道


MaxLiveTimeDeviceLandingServerKey = 60*8


DeviceAccessHttpAPI = 'iot.device.api_server.{}'       # 记录设备接入服务器 {url,landing_time}
DeviceServerRel = 'iot.device_server_rel'     # 存放所有上线设备与接入服务器的关联关系

SensorStatusHash= 'iot.sensor.status.{device_id}.{sensor_type}.{sensor_id}' # {device_id}_{sensor_type}_{sensor_id}'
DeviceStatusHash = 'iot.device.status.{device_id}'

AppRequestAuthCodeWidthIdsPrefix = 'iot.authcode.ids.'
AppRequestAuthCodePrefix = 'iot.authcode.data.'