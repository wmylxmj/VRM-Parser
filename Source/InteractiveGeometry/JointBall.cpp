//
// Created by 13973 on 25-3-24.
//

#include "JointBall.h"

#include <cmath>

#include "../Core/Shader.h"

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(shaderRC);

JointBall::JointBall(const unsigned int numDivisions) {
    GenerateMesh(numDivisions);
    GL_CHECK_ERRORS(SetupMeshToGL())
    GL_CHECK_ERRORS(SetupShaderProgram());
}

void JointBall::SetupShaderProgram() {
    const auto fileSystem = cmrc::shaderRC::get_filesystem();
    const auto vsFile = fileSystem.open("Source/InteractiveGeometry/Shaders/JointBall.vsh");
    GLuint vsh = CompileShader(vsFile.begin(), GL_VERTEX_SHADER);
    const auto fsFile = fileSystem.open("Source/InteractiveGeometry/Shaders/JointBall.fsh");
    GLuint fsh = CompileShader(fsFile.begin(), GL_FRAGMENT_SHADER);
    programID = LinkProgram({ vsh, fsh });
}

void JointBall::SetupMeshToGL() {
    // 创建缓冲区对象
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &instanceVBO);
    glGenBuffers(1, &ebo);

    // 将VBO绑定到VAO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // 传输顶点数据
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(JointBall::Vertex), vertices.data(), GL_STATIC_DRAW);

    // 传输索引数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(JointBall::Vertex), reinterpret_cast<void *>(offsetof(JointBall::Vertex, position)));
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(JointBall::Vertex), reinterpret_cast<void *>(offsetof(JointBall::Vertex, normal)));

    // 实例属性
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    GL_CHECK_ERRORS(glBufferData(GL_ARRAY_BUFFER, sizeof(JointBall::InstanceAttributes) * 200, nullptr, GL_DYNAMIC_DRAW));

    // 实例球心
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(JointBall::InstanceAttributes), reinterpret_cast<void *>(offsetof(JointBall::InstanceAttributes, center)));
    GL_CHECK_ERRORS(glVertexAttribDivisor(2, 1));

    // 实例半径
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(JointBall::InstanceAttributes), reinterpret_cast<void *>(offsetof(JointBall::InstanceAttributes, radius)));
    GL_CHECK_ERRORS(glVertexAttribDivisor(3, 1));

    // 实例颜色
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(JointBall::InstanceAttributes), reinterpret_cast<void *>(offsetof(JointBall::InstanceAttributes, color)));
    GL_CHECK_ERRORS(glVertexAttribDivisor(4, 1));

    glBindVertexArray(0);
}

void JointBall::GenerateMesh(const unsigned int numDivisions) {
    // 北极点
    vertices.push_back({glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)});

    for (unsigned int i = 1; i < numDivisions; i++) {
        const float theta = static_cast<float>(i) * M_PI / numDivisions;
        const float y = std::cos(theta);
        const float sinTheta = std::sin(theta);

        for (unsigned int j = 0; j < numDivisions; j++) {
            const float phi = static_cast<float>(j) * 2.0f * M_PI / numDivisions;
            const float z = sinTheta * std::cos(phi);
            const float x = sinTheta * std::sin(phi);

            vertices.push_back({glm::vec3(x, y, z), glm::vec3(x, y, z)});
        }
    }

    // 南极点
    vertices.push_back({glm::vec3(0, -1, 0), glm::vec3(0, -1, 0)});

    // 北极圈
    for (unsigned int i = 0; i < numDivisions; i++) {
        indices.push_back(0);
        indices.push_back(1 + (i + 1) % numDivisions);
        indices.push_back(1 + i);
    }

    // 中间四边形带
    for (unsigned int i = 0; i < numDivisions-2; i++) {
        const unsigned int currentLineStart = 1 + i * numDivisions;
        const unsigned int nextLineStart = 1 + (i + 1) * numDivisions;
        for (unsigned int j = 0; j < numDivisions; j++) {
            unsigned int topLeft = currentLineStart + j;
            unsigned int topRight = currentLineStart + (j + 1) % numDivisions;
            unsigned int bottomLeft = nextLineStart + j;
            unsigned int bottomRight = nextLineStart + (j + 1) % numDivisions;

            indices.push_back(topLeft);
            indices.push_back(topRight);
            indices.push_back(bottomRight);

            indices.push_back(topLeft);
            indices.push_back(bottomRight);
            indices.push_back(bottomLeft);
        }
    }

    // 南极圈
    for (unsigned int i = 0; i < numDivisions; i++) {
        indices.push_back(1 + (numDivisions - 2) * numDivisions + i);
        indices.push_back(1 + (numDivisions - 2) * numDivisions + (i + 1) % numDivisions);
        indices.push_back(vertices.size() - 1);
    }
}

void JointBall::DrawInstances(const std::vector<InstanceAttributes> &instances, const glm::mat4 &matModel, const glm::mat4 &matView, const glm::mat4 &matProjection) const {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(InstanceAttributes), instances.data());
    glUseProgram(programID);
    SetShaderUniformMat4(programID, "matModel", matModel);
    SetShaderUniformMat4(programID, "matView", matView);
    SetShaderUniformMat4(programID, "matProjection", matProjection);
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr, instances.size());
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
