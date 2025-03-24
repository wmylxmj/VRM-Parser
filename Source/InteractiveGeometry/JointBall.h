//
// Created by 13973 on 25-3-24.
//

#pragma once

#include "../Core/PrecompiledHeader.h"
#include <vector>

class JointBall {

public:

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    explicit JointBall(unsigned int numDivisions = 18);

private:
    void Generate(unsigned int numDivisions);
};



