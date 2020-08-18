cmake_minimum_required(VERSION 2.8)
project(Monkey)

set(CMAKE_CXX_STANDARD 11)
add_definitions( -D_MACOS )
include_directories(/usr/local/include)
include_directories(/usr/local/include /usr/local/Cellar/boost/1.60.0_2/include)
link_directories(/usr/local/lib /usr/local/lib64)
link_libraries(boost_system  jsoncpp pthread nanomsg )
#set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
add_executable(Monkey Controller.cpp  connection.cpp error.cpp logger.cpp  message.cpp  app.cpp config.cpp crc16.c main.cpp service.cpp utils.cpp server.cpp SensorHub.cpp  sensor.cpp
        Controller.h base.h  connection.h error.h logger.h md5.h service.h utils.h  app.h config.h message.h  stringutils.h version.h SensorHub.h sensor.h
        )




