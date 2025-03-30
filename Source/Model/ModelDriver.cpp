//
// Created by 13973 on 25-3-29.
//

#include "ModelDriver.h"
#include <stack>

ModelDriver::ModelDriver(const std::shared_ptr<Model>& pModel) {
    this->pModel = pModel;
}

void ModelDriver::CalcBonesTransformations() {
    // 缓存，避免重复计算
    std::vector<bool> bonesCalculated(pModel->bones.size(), false);

    for (int i = 0; i < pModel->bones.size(); i++) {
        // 如果已计算，则跳过
        if (bonesCalculated[i]) continue;

        Model::Bone bone(pModel->bones[i]);

        unsigned int parentIndex = bone.parentIndex;
        std::stack<unsigned int> parentChain;

        // 溯源到已计算的父节点或根节点
        while (parentIndex != INVALID_PARENT && !bonesCalculated[parentIndex]) {
            parentChain.push(parentIndex);
            parentIndex = pModel->bones[parentIndex].parentIndex;
        }

        while (!parentChain.empty()) {
            parentIndex = parentChain.top();
            parentChain.pop();

            Model::Bone parentBone = pModel->bones[parentIndex];

            if (parentBone.parentIndex == INVALID_PARENT) {
                globalBoneTransformations[parentIndex] = parentBone.transformation;
            }
            else {
                assert(bonesCalculated[parentBone.parentIndex]);
                globalBoneTransformations[parentIndex] = globalBoneTransformations[parentBone.parentIndex] * parentBone.transformation;
            }
            finalBoneTransformations[parentIndex] = globalBoneTransformations[parentIndex] * parentBone.offsetMatrix;
            bonesCalculated[parentIndex] = true;
        }

        // 计算当前骨骼变换矩阵
        if (bone.parentIndex == INVALID_PARENT) {
            globalBoneTransformations[i] = bone.transformation;
        }
        else {
            assert(bonesCalculated[bone.parentIndex]);
            globalBoneTransformations[i] = globalBoneTransformations[bone.parentIndex] * bone.transformation;
        }
        finalBoneTransformations[i] = globalBoneTransformations[i] * bone.offsetMatrix;
        bonesCalculated[i] = true;
    }
}

