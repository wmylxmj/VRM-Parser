#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

void main() {
    gl_position = matProj * matView * matModel * vec4(position, 1.0f);
}
