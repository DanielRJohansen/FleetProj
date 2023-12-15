cmake_minimum_required(VERSION 3.10)
project(FleetManagementSystem)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(Agent)
add_subdirectory(Server)