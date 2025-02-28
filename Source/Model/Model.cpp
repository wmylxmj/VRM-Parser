//
// Created by 13973 on 25-2-28.
//

#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

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

}

