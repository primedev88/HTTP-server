cmake_minimum_required(VERSION 3.2)
project(HTTP_server)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(HTTP_server ${SOURCES})