//
// Created by 13973 on 25-3-22.
//

#pragma once

#include "Model.h"
#include "../Core//PrecompiledHeader.h"

extern void SetupModelToGL(const Model& model, GLuint &vao,GLuint &vbo, GLuint &ebo);

extern std::vector<glm::mat4> CalcBonesFinalTransformations(const Model& model);