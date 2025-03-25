//
// Created by 13973 on 25-3-24.
//

#pragma once

#include "../Core/PrecompiledHeader.h"
#include <vector>

class JointBall {

public:
    GLuint vao{}, vbo{}, ebo{};

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    explicit JointBall(unsigned int numDivisions = 18);

private:
    void GenerateMesh(unsigned int numDivisions);
    void SetupModelToGL();

};



