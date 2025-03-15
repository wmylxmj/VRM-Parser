cmake_minimum_required(VERSION 3.29)
project(VRM_Parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(VRM_Parser Source/main.cpp
        Source/Model/Model.cpp
        Source/Model/Model.h
        Source/Model/VrmModel.cpp
        Source/Model/VrmModel.h)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(VRM_Parser PRIVATE assimp::assimp)

find_package(glm CONFIG REQUIRED)
target_link_libraries(VRM_Parser PRIVATE glm::glm)

find_package(glad CONFIG REQUIRED)
target_link_libraries(VRM_Parser PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(VRM_Parser PRIVATE glfw)
