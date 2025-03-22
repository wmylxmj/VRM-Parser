//
// Created by 13973 on 25-3-22.
//

#include "ModelUtilities.h"
#include <stack>

void SetupModelToGL(const Model& model, GLuint &vao,GLuint &vbo, GLuint &ebo) {
    // 创建缓冲区对象
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // 将VBO绑定到VAO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // 传输顶点数据
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(Vertex), model.vertices.data(), GL_STATIC_DRAW);

    // 传输索引数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(unsigned int), model.indices.data(), GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position)));
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));
    // 顶点骨骼索引 注意对于整型，需要用IPointer
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, NUM_BONES_PER_VERTEX, GL_UNSIGNED_INT, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, boneIndices)));
    // 顶点骨骼权重
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, boneWeights)));

    glBindVertexArray(0);
}

std::vector<glm::mat4> CalcBonesFinalTransformations(const Model& model) {
    // 缓存，避免重复计算
    std::vector<glm::mat4> finalTransformations(model.bones.size());
    std::vector<glm::mat4> globalTransformations(model.bones.size());
    std::vector<bool> bonesCalculated(model.bones.size(), false);

    for (int i = 0; i < model.bones.size(); i++) {
        // 如果已计算，则跳过
        if (bonesCalculated[i]) continue;

        Bone bone(model.bones[i]);

        unsigned int parentIndex = bone.parentIndex;
        std::stack<unsigned int> parentChain;

        // 溯源到已计算的父节点或根节点
        while (parentIndex != INVALID_PARENT && !bonesCalculated[parentIndex]) {
            parentChain.push(parentIndex);
            parentIndex = model.bones[parentIndex].parentIndex;
        }

        while (!parentChain.empty()) {
            parentIndex = parentChain.top();
            parentChain.pop();

            Bone parentBone = model.bones[parentIndex];

            if (parentBone.parentIndex == INVALID_PARENT) {
                globalTransformations[parentIndex] = parentBone.transformation;
            }
            else {
                assert(bonesCalculated[parentBone.parentIndex]);
                globalTransformations[parentIndex] = globalTransformations[parentBone.parentIndex] * parentBone.transformation;
            }
            finalTransformations[parentIndex] = globalTransformations[parentIndex] * parentBone.offsetMatrix;
            bonesCalculated[parentIndex] = true;
        }

        // 计算当前骨骼变换矩阵
        if (bone.parentIndex == INVALID_PARENT) {
            globalTransformations[i] = bone.transformation;
        }
        else {
            assert(bonesCalculated[bone.parentIndex]);
            globalTransformations[i] = globalTransformations[bone.parentIndex] * bone.transformation;
        }
        finalTransformations[i] = globalTransformations[i] * bone.offsetMatrix;
        bonesCalculated[i] = true;
    }

    return finalTransformations;
}


