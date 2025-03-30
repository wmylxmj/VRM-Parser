//
// Created by 13973 on 25-3-29.
//

#pragma once

#include <memory>
#include "../Core/PrecompiledHeader.h"
#include "Model.h"

class ModelDriver {

public:
    std::shared_ptr<Model> pModel;

    std::vector<glm::mat4> finalBoneTransformations;
    std::vector<glm::mat4> globalBoneTransformations;

    explicit ModelDriver(const std::shared_ptr<Model>& pModel);

    void CalcBonesTransformations();


};
