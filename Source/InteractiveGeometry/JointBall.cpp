//
// Created by 13973 on 25-3-24.
//

#include "JointBall.h"

#include <cmath>

JointBall::JointBall(const unsigned int numDivisions) {
    GenerateMesh(numDivisions);
}

void JointBall::GenerateMesh(unsigned int numDivisions) {
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


