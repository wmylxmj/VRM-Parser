//
// Created by 13973 on 25-2-28.
//

#pragma once

#include <string>
#include "assimp/scene.h"

class Model {

public:
    explicit Model(const std::string& filePath);

private:
    void SceneProcessing(const aiScene* scene);
    void NodeProcessing(const aiNode *node, const aiScene *scene);

    static bool BoneHasWeights(const aiBone* bone);

protected:
    std::string m_directory;
};
