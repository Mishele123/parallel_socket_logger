cmake_minimum_required(VERSION 3.15)
project(work_with_net)

set(CMAKE_CXX_STANDARD 17)



add_executable(client src/client.cpp)

add_executable(server src/server.cpp) 