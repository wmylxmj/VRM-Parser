//
// Created by 13973 on 25-2-28.
//

#pragma once

#include <string>
#include <vector>
#include <map>

#include "assimp/scene.h"
#include "glm/glm.hpp"

#define NUM_BONES_PER_VERTEX 4

#define INVALID_PARENT 0XFFFFFFFF

typedef struct {
    glm::vec3 position;
    glm::vec3 normal;

    // 骨骼索引和权重
    unsigned int boneIndices[NUM_BONES_PER_VERTEX];
    float boneWeights[NUM_BONES_PER_VERTEX];
} Vertex;

typedef struct {
    unsigned int parentIndex;
    glm::mat4 offsetMatrix;
} Bone;

typedef struct {
    unsigned int vertexBase;
    unsigned int indexBase;
    unsigned int numIndices;
} MeshEntry;

class Model {

public:
    std::vector<Bone> bones;
    std::map<std::string, unsigned int> boneIndexMapping; // maps a bone name to its index
    std::vector<MeshEntry> meshEntries;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    explicit Model(const std::string& filePath);

private:
    void SceneProcessing(const aiScene* pScene);
    void NodeProcessing(const aiNode *pNode, const aiScene *pScene);
    void MeshProcessing(const aiMesh *pMesh, const aiScene *pScene);
    void BoneProcessing(const aiBone *pBone, const MeshEntry &meshEntry);

protected:
    std::string m_directory;
};
