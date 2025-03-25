#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in mat4 matModel;
layout (location = 3) in vec4 color;

uniform mat4 matProjection;
uniform mat4 matView;

out vec4 vColor;

void main() {
    gl_Position = matProjection * matView * matModel * vec4(position, 1.0f);
    vColor = color;
}
