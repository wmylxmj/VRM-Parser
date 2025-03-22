#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in uvec4 boneIndices;
layout (location = 3) in vec4 boneWeights;

uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

layout(std140, binding = 0) uniform  BoneTransforms {
    mat4 matBones[200];
};

out vec3 normalOut;

void main() {
    vec4 positionL = vec4(position, 1.0f);
    vec4 normalL = vec4(normal, 0.0f);

    if (boneWeights[0] + boneWeights[1] + boneWeights[2] + boneWeights[3] > 1e-3) {

        mat4 matBone = matBones[boneIndices[0]] * boneWeights[0];
        matBone += matBones[boneIndices[1]] * boneWeights[1];
        matBone += matBones[boneIndices[2]] * boneWeights[2];
        matBone += matBones[boneIndices[3]] * boneWeights[3];

        positionL = matBone * positionL;
        normalL = matBone * normalL;
    }

    gl_Position = matProjection * matView * matModel * positionL;
    normalOut = (matProjection * matView * matModel * normalL).xyz;
}
