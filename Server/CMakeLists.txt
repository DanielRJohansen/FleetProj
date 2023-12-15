cmake_minimum_required(VERSION 3.10)
project(Server)

# Assuming you have cpp-httplib library downloaded and placed in a directory
include_directories(../lib/)

# Add source files
add_executable(Server Server.cpp)