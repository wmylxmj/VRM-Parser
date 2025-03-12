//
// Created by 13973 on 25-2-28.
//

#pragma once

#include <string>
#include <vector>
#include <map>

#include "assimp/scene.h"

#define INVALID_PARENT 0XFFFFFFFF

typedef struct {
    float position[3];
    float normal[3];
} Vertex;

typedef struct {
    unsigned int parentIndex;
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
    void BoneProcessing(const aiBone *pBone, const aiScene *pScene);

protected:
    std::string m_directory;
};
