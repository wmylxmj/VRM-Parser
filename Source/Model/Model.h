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


class Model {
public:

    typedef struct {
        glm::vec3 position;
        glm::vec3 normal;

        // 骨骼索引和权重
        unsigned int boneIndices[NUM_BONES_PER_VERTEX];
        float boneWeights[NUM_BONES_PER_VERTEX];
    } Vertex;

    typedef struct {
        std::string name;
        unsigned int parentIndex;
        glm::mat4 offsetMatrix;
        glm::mat4 transformation;
        glm::mat4 bindingPoseTransformation;
    } Bone;

    typedef struct {
        unsigned int vertexBase;
        unsigned int indexBase;
        unsigned int numIndices;
    } MeshEntry;


    std::vector<Bone> bones;
    std::map<std::string, unsigned int> boneNameIndexMapping; // maps a bone name to its index
    std::vector<MeshEntry> meshEntries;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    explicit Model(const std::string& filePath);

private:
    void SceneProcessing(const aiScene* pScene);
    void NodeProcessing(const aiNode *pNode, glm::mat4 rootTransformation = glm::mat4(1.0f), unsigned int parentIndex = INVALID_PARENT);
    void MeshProcessing(const aiMesh *pMesh, const aiScene *pScene);
    void BoneProcessing(const aiBone *pBone, const MeshEntry &meshEntry);

protected:
    std::string m_directory;
};

