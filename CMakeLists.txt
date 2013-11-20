cmake_minimum_required(VERSION 2.8)

PROJECT(Programming_challenger)

SET(CMAKE_CXX_COMPILER g++)

SET(CMAKE_CXX_FLAGS "-g -Wall")

MESSAGE(STATUS "this is" ${Programming_challenger_SOURCE_DIR})

INCLUDE_DIRECTORIES(${Programming_challenger_SOURCE_DIR}/problems)

SET(EXECUTABLE_OUTPUT_PATH ${Programming_challenger_SOURCE_DIR}/bin)

SET(SRC_LIST main.cpp)

ADD_EXECUTABLE(Programming_challenger ${SRC_LIST})

