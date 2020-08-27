cmake_minimum_required(VERSION 2.8)
project(TestDev)

set(CMAKE_CXX_STANDARD 11)
add_definitions( -D_MACOS -fpermissive -D_SDF_IMPL )
include_directories(/usr/local/include)
include_directories(/usr/local/include /usr/local/Cellar/boost/1.60.0_2/include )
link_directories(/usr/local/lib /usr/local/lib64)
link_libraries(sdf_crypto pthread)


add_executable(sdf_tool main.cpp ../sdf-utils.h ../sdf-utils.cpp )





