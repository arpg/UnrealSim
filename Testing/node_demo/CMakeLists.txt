project(NodeDemo)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")

find_package(Node REQUIRED)
include_directories(${Node_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(node_demo node_demo.cpp ImageMessage.pb.cpp InputMessage.pb.cpp)
target_link_libraries(node_demo ${OpenCV_LIBS} ${Node_LIBRARIES})
