//
// Created by 13973 on 25-2-28.
//

#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <cassert>
#include <vector>

Model::Model(const std::string& filePath) {
    Assimp::Importer importer;
    constexpr unsigned int flags = aiProcess_Triangulate |
                                   aiProcess_JoinIdenticalVertices |
                                   aiProcess_GenNormals;

    const aiScene *scene = importer.ReadFile(filePath, flags);

    if (scene == nullptr) {
        std::cerr << importer.GetErrorString() << std::endl;
        return;
    }

    // 获得模型文件根目录
    m_directory = filePath.substr(0, filePath.find_last_of("/\\") + 1);

    SceneProcessing(scene);
}

void Model::SceneProcessing(const aiScene *scene) {
    NodeProcessing(scene->mRootNode, scene);
}

// 查看骨骼是否有不为0的权重，如果不是，则该骨骼无效
bool Model::BoneHasWeights(const aiBone *bone) {
    for (int i = 0; i < bone->mNumWeights; ++i) {
        if (bone->mWeights[i].mWeight > 0) {
            return true;
        }
    }
    return false;
}

void Model::NodeProcessing(const aiNode *node, const aiScene *scene) {
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        std::cout << "processing mesh " << mesh->mName.data << std::endl;
        std::pmr::vector<std::string> boneNames;
        for (int j = 0; j < mesh->mNumBones; ++j) {
            if (BoneHasWeights(mesh->mBones[j])) {
                boneNames.emplace_back(mesh->mBones[j]->mName.data);
            }
        }
        std::cout << "numBones: " << boneNames.size() << std::endl;
        for (const auto& name : boneNames) {
            std::cout << name << " ";
        }
        std::cout << std::endl;
    }
    // 处理子节点
    for (int i = 0; i < node->mNumChildren; ++i) {
        NodeProcessing(node->mChildren[i], scene);
    }
}
