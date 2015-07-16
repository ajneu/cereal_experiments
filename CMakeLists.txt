cmake_minimum_required(VERSION 3.2)

project(go)

set(TARGET go)
set(SOURCES go.cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(../cereal/include)

add_executable(${TARGET} ${SOURCES})