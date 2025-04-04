cmake_minimum_required(VERSION 3.29)
project(VRM_Parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
        Source/main.cpp
        Source/Model/Model.cpp
        Source/Model/Model.h
        Source/Model/VrmModel.cpp
        Source/Model/VrmModel.h
        Source/Core/PrecompiledHeader.h
        Source/Core/IApplication.cpp
        Source/Core/IApplication.h
        Source/Core/Camera.cpp
        Source/Core/Camera.h
        Source/Core/Shader.cpp
        Source/Core/Shader.h
        ThirdParty/tinygltf/json.hpp
        ThirdParty/tinygltf/stb_image.h
        ThirdParty/tinygltf/stb_image_write.h
        ThirdParty/tinygltf/tiny_gltf.h
        Source/Model/ModelUtilities.cpp
        Source/Model/ModelUtilities.h
        Source/InteractiveGeometry/JointBall.cpp
        Source/InteractiveGeometry/JointBall.h
        Source/Model/ModelDriver.cpp
        Source/Model/ModelDriver.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

include(ThirdParty/cmrc/CMakeRC.cmake)
cmrc_add_resource_library(shaderRC
        Source/Shaders/Shader.vsh
        Source/Shaders/Shader.fsh
        Source/InteractiveGeometry/Shaders/JointBall.vsh
        Source/InteractiveGeometry/Shaders/JointBall.fsh
)
target_link_libraries(${PROJECT_NAME} PRIVATE shaderRC)
