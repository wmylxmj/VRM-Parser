//
// Created by 13973 on 25-2-28.
//

#pragma once

#include <string>
#include <vector>

#include "assimp/scene.h"

typedef struct {
    float position[3];
    float normal[3];
} Vertex;

typedef struct {

} Bone;

typedef struct {
    unsigned int vertexBase;
    unsigned int indexBase;
    unsigned int numIndices;
} MeshEntries;

class Model {

public:
    std::vector<Bone> bones;

    explicit Model(const std::string& filePath);

private:
    void SceneProcessing(const aiScene* scene);
    void NodeProcessing(const aiNode *node, const aiScene *scene);

    static bool BoneHasWeights(const aiBone* bone);

protected:
    std::string m_directory;
};
