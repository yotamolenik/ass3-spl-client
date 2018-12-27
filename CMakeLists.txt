cmake_minimum_required(VERSION 3.13)
project(Client)

set(CMAKE_CXX_STANDARD 11)

add_executable(Client main.cpp src/connectionHandler.cpp src/echoClient.cpp src/echoClient.h include/connectionHandler.h)