//
// Created by 13973 on 25-2-28.
//

#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
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
    for (unsigned int i = 0; i < pScene->mNumMeshes; ++i) {
        const aiMesh *pMesh = pScene->mMeshes[i];
        MeshProcessing(pMesh, pScene);
    }
    // 处理场景层次结构
    NodeProcessing(pScene->mRootNode, pScene);
    std::cout << boneIndexMapping.size() << std::endl;
    for (const auto&[fst, snd] : boneIndexMapping) {
        std::cout << fst << ": " << snd << "->" << bones[snd].parentIndex << std::endl;
    }
    std::cout << vertices.size() << std::endl;
    std::cout << indices.size() / 3 << std::endl;
    std::cout << meshEntries.size() << std::endl;
}

void Model::MeshProcessing(const aiMesh *pMesh, const aiScene *pScene) {
    // 网格入口初始化
    MeshEntry meshEntry;
    meshEntry.vertexBase = vertices.size();
    meshEntry.indexBase = indices.size();
    meshEntry.numIndices = 0;
    // 加载顶点
    for (unsigned int i = 0; i < pMesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position[0] = pMesh->mVertices[i].x;
        vertex.position[1] = pMesh->mVertices[i].y;
        vertex.position[2] = pMesh->mVertices[i].z;
        vertex.normal[0] = pMesh->mNormals[i].x;
        vertex.normal[1] = pMesh->mNormals[i].y;
        vertex.normal[2] = pMesh->mNormals[i].z;
        vertices.push_back(vertex);
    }
    // 加载三角形索引
    for(unsigned int i = 0; i < pMesh->mNumFaces; i++) {
        if (const aiFace face = pMesh->mFaces[i]; face.mNumIndices == 3) {
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(meshEntry.vertexBase + face.mIndices[j]);
                meshEntry.numIndices ++;
            }
        }
    }
    meshEntries.push_back(meshEntry);
    // 加载骨骼
    for (unsigned int i = 0; i < pMesh->mNumBones; ++i) {
        const aiBone *pBone = pMesh->mBones[i];
        BoneProcessing(pBone, meshEntry);
    }
}

void Model::BoneProcessing(const aiBone *pBone, const MeshEntry &meshEntry) {
    const std::string boneName(pBone->mName.data);
    const unsigned int boneIndex = boneIndexMapping.contains(boneName) ? boneIndexMapping[boneName] : bones.size();

    bool boneHasWeights = false; // 用于去除权重全为0的无效骨骼
    for (unsigned int i = 0; i < pBone->mNumWeights; ++i) {
        if (pBone->mWeights[i].mWeight > 0) {
            boneHasWeights = true;
            // 载入顶点的骨骼索引和权重

        }
    }
    // 如果是新的有效骨骼，那么添加
    if (boneHasWeights && boneIndex == bones.size()) {
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
    for (unsigned int i = 0; i < pNode->mNumChildren; ++i) {
        NodeProcessing(pNode->mChildren[i], pScene);
    }
}
