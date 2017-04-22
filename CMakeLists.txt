cmake_minimum_required(VERSION 3.7)
project(zRSA)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        zrsa.cpp)

add_executable(zRSA ${SOURCE_FILES})