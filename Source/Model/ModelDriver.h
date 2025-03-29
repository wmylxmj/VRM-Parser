//
// Created by 13973 on 25-3-29.
//

#pragma once

#include <memory>
#include "../Core/PrecompiledHeader.h"
#include "Model.h"

class ModelDriver {

public:
    std::shared_ptr<Model> model;

    std::vector<glm::mat4> finalBoneTransformations;

};
