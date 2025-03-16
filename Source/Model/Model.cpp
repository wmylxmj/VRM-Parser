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
    // 处理网格
    for (unsigned int i = 0; i < pScene->mNumMeshes; ++i) {
        const aiMesh *pMesh = pScene->mMeshes[i];
        MeshProcessing(pMesh, pScene);
    }
    // 处理场景层次结构
    NodeProcessing(pScene->mRootNode);

    // 后处理：归一化顶点骨骼权重
    for (unsigned int i = 0; i < vertices.size(); ++i) {
        float totalWeight = 0.0f;
        for (const float boneWeight : vertices[i].boneWeights) {
            totalWeight += boneWeight;
        }
        if (totalWeight > 0) {
            for (float & boneWeight : vertices[i].boneWeights) {
                boneWeight /= totalWeight;
            }
        }
    }
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
        vertex.position.x = pMesh->mVertices[i].x;
        vertex.position.y = pMesh->mVertices[i].y;
        vertex.position.z = pMesh->mVertices[i].z;
        vertex.normal.x = pMesh->mNormals[i].x;
        vertex.normal.y = pMesh->mNormals[i].y;
        vertex.normal.z = pMesh->mNormals[i].z;
        // 骨骼数据初始化
        for (unsigned int j = 0; j < NUM_BONES_PER_VERTEX; ++j) {
            vertex.boneIndices[j] = 0;
            vertex.boneWeights[j] = 0.0f;
        }
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
            const unsigned int vertexIndex = meshEntry.vertexBase + pBone->mWeights[i].mVertexId;
            // 查找空的插槽
            unsigned int slot = 0;
            float minWeight = std::numeric_limits<float>::max();
            bool exceedingBoneCountWarning = true;
            for (int j = 0; j < NUM_BONES_PER_VERTEX; ++j) {
                // 如果有空位
                if (vertices[vertexIndex].boneWeights[j] == 0) {
                    exceedingBoneCountWarning = false;
                    slot = j;
                    break;
                }
                // 如果没有空位，替换最小权重的槽位
                if (vertices[vertexIndex].boneWeights[j] < minWeight) {
                    minWeight = vertices[vertexIndex].boneWeights[j];
                    slot = j;
                }
            }
            if (exceedingBoneCountWarning) {
                std::cerr << "Warning: Vertex " << vertexIndex << "Has Too Many Bones" << std::endl;
            }
            // 分配骨骼索引和权重
            vertices[vertexIndex].boneIndices[slot] = boneIndex;
            vertices[vertexIndex].boneWeights[slot] = pBone->mWeights[i].mWeight;
        }
    }

    // 如果是新的有效骨骼，那么添加
    if (boneHasWeights && boneIndex == bones.size()) {
        Bone bone;
        bone.name = boneName;
        bone.parentIndex = INVALID_PARENT;
        memcpy(&bone.offsetMatrix, &pBone->mOffsetMatrix.a1, sizeof(aiMatrix4x4));
        bone.offsetMatrix = glm::transpose(bone.offsetMatrix);
        bone.transformation = glm::inverse(bone.offsetMatrix); // 绑定姿势
        boneIndexMapping[boneName] = bones.size();
        bones.push_back(bone);
    }
}

void Model::NodeProcessing(const aiNode *pNode, glm::mat4 rootTransformation, unsigned int parentIndex) {
    // 如果该节点是骨骼节点，更新父索引
    if (const std::string nodeName(pNode->mName.data); boneIndexMapping.contains(nodeName)) {
        bones[boneIndexMapping[nodeName]].parentIndex = parentIndex;
        parentIndex = boneIndexMapping[nodeName];
    }
    // 否则更新骨骼的根变换
    else {
        glm::mat4 nodeTransformation;
        memcpy(&nodeTransformation, &pNode->mTransformation, sizeof(aiMatrix4x4));
        nodeTransformation = glm::inverse(nodeTransformation);
        rootTransformation = rootTransformation * nodeTransformation;
    }
    // 处理子节点
    for (unsigned int i = 0; i < pNode->mNumChildren; ++i) {
        NodeProcessing(pNode->mChildren[i], rootTransformation, parentIndex);
    }
}

