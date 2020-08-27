cmake_minimum_required(VERSION 2.8)
project(Monkey)

set(CMAKE_CXX_STANDARD 11)
add_definitions( -D_MACOS -fpermissive -D_SDF_IMPL)
include_directories(/usr/local/include)
include_directories(/usr/local/include /usr/local/Cellar/boost/1.60.0_2/include )
link_directories(/usr/local/lib /usr/local/lib64)
link_libraries(boost_system  jsoncpp pthread nanomsg sdf_crypto)
aux_source_directory(GM50 SRCS_GM50)
#set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
add_executable(Monkey CowServer.cpp Controller.cpp  connection.cpp error.cpp logger.cpp  message.cpp
        app.cpp config.cpp crc16.c
        service.cpp utils.cpp server.cpp
#        SensorHub.cpp  sensor.cpp
        Controller.h base.h  connection.h error.h logger.h md5.h service.h utils.h
        app.h config.h message.h  stringutils.h version.h
#        SensorHub.h sensor.h
       ${SRCS_GM50} GM50/message.h
        GM50/message.cpp GM50/envelope.h GM50/envelope.cpp GM50/datatype.h GM50/object.cpp
        GM50/serialize.h GM50/attribute.h GM50/attribute.cpp GM50/bytearray.h
        GM50/utils.h GM50/error.h GM50/command.h GM50/command.cpp GM50/apps/SecKeyManagement.cpp
        GM50/apps/SecKeyManagement.h GM50/apps/RemoteMonitor.cpp GM50/apps/RemoteMonitor.h
        GM50/apps/ParamConfigure.cpp GM50/apps/ParamConfigure.h GM50/apps/RemoteMaintenance.cpp
        GM50/apps/RemoteMaintenance.h GM50/apps/ComplianceTest.cpp GM50/apps/ComplianceTest.h
        GM50/apps/VpnManagement.cpp GM50/apps/VpnManagement.h
        ManagedAgent.cpp ManagedAgent.h
        bundles/DeviceManagementImpl.cpp bundles/DeviceManagementImpl.h
        bundles/SecKeyManagementImpl.cpp bundles/SecKeyManagementImpl.h bundles/RemoteMonitorImpl.cpp
        bundles/RemoteMonitorImpl.h bundles/ParamConfigureImpl.cpp bundles/ParamConfigureImpl.h
        bundles/RemoteMaintenaceImpl.cpp bundles/RemoteMaintenaceImpl.h bundles/ComplianceTestImpl.cpp
        bundles/ComplianceTestImpl.h bundles/VpnManagementImpl.cpp bundles/VpnManagementImpl.h
        bundles/base.h bundles/all.h sdf-utils.h sdf-utils.cpp GM50/gm0006.h GM50/apps/UserAppJsonMessage.cpp
        GM50/apps/UserAppJsonMessage.h bundles/UserAppJsonDataImpl.cpp bundles/UserAppJsonDataImpl.h

#        SecChanGateServer/Upstream.cpp SecChanGateServer/Upstream.h sdf-impl.h UserSecChannel.cpp
#        UserSecChannel.h
        Connector.cpp Connector.h Direction.h DirectionIn.cpp DirectionIn.h
        DirectionOut.cpp DirectionOut.h Direction.cpp )

#add_subdirectory(GM50)
#target_link_libraries(Monkey GM50)




