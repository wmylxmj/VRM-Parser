//
// Created by 13973 on 25-3-24.
//

#pragma once

#include "../Core/PrecompiledHeader.h"
#include <vector>

class JointBall {

public:
    GLuint vao{}, vbo{}, ebo{};
    GLuint instanceVBO{};

    GLuint programID;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct InstanceAttributes {
        glm::mat4 transformation;
        glm::vec4 color;
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    explicit JointBall(unsigned int numDivisions = 18);

private:
    void GenerateMesh(unsigned int numDivisions);
    void SetupMeshToGL();
    void SetupShaderProgram();
    void DrawInstances(const std::vector<InstanceAttributes> &instances, const glm::mat4 &matModel, const glm::mat4 &matView, const glm::mat4 &matProjection) const;
};
