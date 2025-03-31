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
        glm::vec3 center;
        float radius;
        glm::vec4 color;
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    explicit JointBall(unsigned int numDivisions = 18);

    void DrawInstances(const std::vector<InstanceAttributes> &instances, const glm::mat4 &matModel, const glm::mat4 &matView, const glm::mat4 &matProjection) const;

private:
    void GenerateMesh(unsigned int numDivisions);
    void SetupMeshToGL();
    void SetupShaderProgram();
};

