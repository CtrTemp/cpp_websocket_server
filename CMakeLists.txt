
# C++ 最低版本
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 指定总工程名
project(WebSocketServer LANGUAGES CXX)

# # 允许GDB调试
# set(CMAKE_BUILD_TYPE DEBUG)

include_directories(. ./include)




aux_source_directory(.  MAIN_SRC_LIST)
aux_source_directory(./src  MAIN_SRC_LIST)
aux_source_directory(./proto  MAIN_SRC_LIST)

message(STATUS ${MAIN_SRC_LIST})

add_executable(${PROJECT_NAME} ${MAIN_SRC_LIST})

# 这里对于 header only 的库应该不需要添加
target_link_libraries(${PROJECT_NAME} libjsoncpp.so libprotobuf.a pthread)


