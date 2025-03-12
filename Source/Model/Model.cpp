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

    const aiScene *pScene = importer.ReadFile(filePath, flags);

    if (pScene == nullptr) {
        std::cerr << importer.GetErrorString() << std::endl;
        return;
    }

    // 获得模型文件根目录
    m_directory = filePath.substr(0, filePath.find_last_of("/\\") + 1);

    SceneProcessing(pScene);
}

void Model::SceneProcessing(const aiScene *pScene) {
    for (int i = 0; i < pScene->mNumMeshes; ++i) {
        const aiMesh *pMesh = pScene->mMeshes[i];
        MeshProcessing(pMesh, pScene);
    }
    // 处理场景层次结构
    NodeProcessing(pScene->mRootNode, pScene);
    std::cout << boneIndexMapping.size() << std::endl;
    for (const auto&[fst, snd] : boneIndexMapping) {
        std::cout << fst << ": " << snd << "->" << bones[snd].parentIndex << std::endl;
    }
}

void Model::MeshProcessing(const aiMesh *pMesh, const aiScene *pScene) {
    MeshEntry meshEntry;
    meshEntry.vertexBase = vertices.size();
    meshEntry.indexBase = indices.size();
    for (int i = 0; i < pMesh->mNumBones; ++i) {
        const aiBone *pBone = pMesh->mBones[i];
        BoneProcessing(pBone, pScene);
    }
}

void Model::BoneProcessing(const aiBone *pBone, const aiScene *pScene) {
    if (!BoneHasWeights(pBone)) return;
    if (const std::string boneName(pBone->mName.data); !boneIndexMapping.contains(boneName)) {
        Bone bone;
        bone.parentIndex = INVALID_PARENT;
        boneIndexMapping[boneName] = bones.size();
        bones.push_back(bone);
    }
}

void Model::NodeProcessing(const aiNode *pNode, const aiScene *pScene) {
    if (const std::string nodeName(pNode->mName.data); boneIndexMapping.contains(nodeName)) {
        if (boneIndexMapping.contains(pNode->mParent->mName.data))
            bones[boneIndexMapping[nodeName]].parentIndex = boneIndexMapping[pNode->mParent->mName.data];
        else {
            std::cerr << "Node " << nodeName << " Parent " << pNode->mParent->mName.data << " not found" << std::endl;
        }
    }
    // 处理子节点
    for (int i = 0; i < pNode->mNumChildren; ++i) {
        NodeProcessing(pNode->mChildren[i], pScene);
    }
}

// 查看骨骼是否有不为0的权重，如果不是，则该骨骼无效
bool Model::BoneHasWeights(const aiBone *pBone) {
    for (int i = 0; i < pBone->mNumWeights; ++i) {
        if (pBone->mWeights[i].mWeight > 0) {
            return true;
        }
    }
    return false;
}
