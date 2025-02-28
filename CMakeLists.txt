cmake_minimum_required(VERSION 3.29)
project(VRM_Parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(VRM_Parser Source/main.cpp
        Source/Model/Model.cpp
        Source/Model/Model.h)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(VRM_Parser PRIVATE assimp::assimp)