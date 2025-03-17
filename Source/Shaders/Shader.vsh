#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

out vec3 normalOut;

void main() {
    gl_Position = matProjection * matView * matModel * vec4(position, 1.0f);
    normalOut = (matProjection * matView * matModel * vec4(normal, 0.0f)).xyz;
}
