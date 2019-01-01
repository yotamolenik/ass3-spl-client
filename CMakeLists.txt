cmake_minimum_required(VERSION 3.13)
project(Client)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)
find_package(Boost 1.58.0 REQUIRED COMPONENTS
        thread filesystem )
include_directories(${Boost_INCLUDE_DIRS})

add_executable(Client main.cpp
        src/connectionHandler.cpp
        include/connectionHandler.h
        src/ReceiveMessages.cpp
        src/ReceiveMessages.h)
target_link_libraries(Client ${Boost_LIBRARIES})