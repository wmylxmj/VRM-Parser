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


std::string VrmModel::ExtractVRMJson(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    // 检查头部 （魔数，版本号，文件长度）
    std::string magic(sizeof(unsigned int), '\0');
    file.read(magic.data(), sizeof(unsigned int));
    assert(magic == "glTF");
    unsigned int version;
    file.read(reinterpret_cast<char*>(&version), sizeof(unsigned int));
    assert(version == 2);
    file.ignore(sizeof(unsigned int)); // 跳过文件长度

    // 检查 JSON 块
    unsigned int chunkLength;
    file.read(reinterpret_cast<char*>(&chunkLength), sizeof(unsigned int));
    // 检查块类型
    std::string chunkType(sizeof(unsigned int), '\0');
    file.read(chunkType.data(), sizeof(unsigned int));
    assert(chunkType == "JSON");
    // 读取 JSON
    std::string json(chunkLength, '\0');
    file.read(&json[0], chunkLength);
    return json;
}

VrmModel::VrmModel(const std::string& filePath) : Model(filePath) {
    auto vrmJson = nlohmann::json::parse(ExtractVRMJson(filePath));

    for (int i = 0; i < vrmJson["extensions"]["VRM"]["humanoid"]["humanBones"].size(); i++) {
        auto humanBone = vrmJson["extensions"]["VRM"]["humanoid"]["humanBones"][i];
        auto node = vrmJson["nodes"][static_cast<int>(humanBone["node"])];
        humanBoneNameIndexMapping[humanBone["bone"]] = boneNameIndexMapping[node["name"]];
    }
}
