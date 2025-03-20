//
// Created by 13973 on 25-2-28.
//

#include "VrmModel.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#include <nlohmann/json.hpp>

#include <iostream>

// 从 GLB 文件中提取 JSON
std::string ExtractVRMJson(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    file.seekg(12); // 跳过 GLB 头部
    uint32_t jsonLength;
    file.read(reinterpret_cast<char*>(&jsonLength), 4);
    file.ignore(4); // 跳过块类型
    std::string json(jsonLength, '\0');
    file.read(&json[0], jsonLength);
    return json;
}


VrmModel::VrmModel(const std::string& filePath) : Model(filePath) {
    std::string json = ExtractVRMJson(filePath);
    //std::cout << json << std::endl;
    auto gltf = nlohmann::json::parse(json);
    //std::cout << gltf["nodes"][3] << std::endl;
    //std::cout << gltf["extensions"]["VRM"]["humanoid"]["humanBones"] << std::endl;

    for (int i = 0; i < gltf["extensions"]["VRM"]["humanoid"]["humanBones"].size(); i++) {
        auto humanBone = gltf["extensions"]["VRM"]["humanoid"]["humanBones"][i];
        auto node = gltf["nodes"][static_cast<int>(humanBone["node"])];
        std::cout << humanBone["bone"] << "->" << node["name"] << "->" << boneNameIndexMapping[node["name"]] << std::endl;
    }

    for (auto& [key, value] : gltf["extensions"]["VRM"]["humanoid"]["hasTranslationDoF"].items()) {
        std::cout << key << std::endl;
    }
    std::cout << gltf["extensions"]["VRM"]["humanoid"] << std::endl;

    // tinygltf::Model model;
    // tinygltf::TinyGLTF loader;
    // std::string err, warn;
    //
    // // 加载 VRM/glTF 文件
    // bool success = loader.LoadBinaryFromFile(&model, &err, &warn, filePath);
    // if (!warn.empty()) std::cerr << "WARNING: " << warn << std::endl;
    // if (!err.empty()) std::cerr << "ERROR: " << err << std::endl;
    // if (!success) return;
}
