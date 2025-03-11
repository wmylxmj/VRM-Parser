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
    for (int i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh *mesh = scene->mMeshes[i];
        for (int j = 0; j < mesh->mNumBones; ++j) {
            if (BoneHasWeights(mesh->mBones[j])) {
                std::string boneName(mesh->mBones[j]->mName.data);
                if (!boneIndexMapping.contains(boneName)) {
                    Bone bone;
                    bone.parentIndex = INVALID_PARENT;
                    boneIndexMapping[boneName] = bones.size();
                    bones.push_back(bone);
                }
            }
        }
    }
    NodeProcessing(scene->mRootNode, scene);
    std::cout << boneIndexMapping.size() << std::endl;
    for (const auto& pair : boneIndexMapping) {
        std::cout << pair.first << ": " << pair.second << "->" << bones[pair.second].parentIndex << std::endl;
    }
}

void Model::NodeProcessing(const aiNode *node, const aiScene *scene) {
    std::string nodeName(node->mName.data);
    if (boneIndexMapping.find(nodeName) != boneIndexMapping.end()) {
        if (boneIndexMapping.find(node->mParent->mName.data) != boneIndexMapping.end())
            bones[boneIndexMapping[nodeName]].parentIndex = boneIndexMapping[node->mParent->mName.data];
        else {
            std::cerr << "Node " << nodeName << " Parent " << node->mParent->mName.data << " not found" << std::endl;
        }
    }
    std::cout << "Node processing " << node->mName.data << std::endl;
    std::cout << "Node Parent: " << (node->mParent == nullptr ? "" : node->mParent->mName.data) << std::endl;
    for (int i = 0; i < node->mNumMeshes; ++i) {
        const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        for (int j = 0; j < mesh->mNumBones; ++j) {
            if (BoneHasWeights(mesh->mBones[j])) {
                std::cout << "Bone " << mesh->mBones[j]->mName.data << std::endl;
            }
        }
    }
    // 处理子节点
    for (int i = 0; i < node->mNumChildren; ++i) {
        NodeProcessing(node->mChildren[i], scene);
    }
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
