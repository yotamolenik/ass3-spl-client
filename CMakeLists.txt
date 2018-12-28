cmake_minimum_required(VERSION 3.13)
project(Client)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)
find_package(Boost COMPONENTS
        system filesystem REQUIRED)
include_directories($
        Boost_INCLUDE_DIRS)

add_executable(Client main.cpp src/connectionHandler.cpp include/connectionHandler.h src/ReciveMessages.cpp src/ReciveMessages.h)