cmake_minimum_required(VERSION 3.10)
project(Agent)

# Include directories, if any
include_directories(../lib/)

# Add source files
add_executable(Agent Agent.cpp)