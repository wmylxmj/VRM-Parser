#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 2) in vec3 center;
layout (location = 3) in float radius;
layout (location = 4) in vec4 color;

uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

out vec4 vColor;

void main() {
    mat4 transformation = mat4(
    radius, 0, 0, 0,
    0, radius, 0, 0,
    0, 0, radius, 0,
    center.x, center.y, center.z, 1);
    gl_Position = matProjection * matView * matModel * transformation * vec4(position, 1.0f);
    vColor = color;
}
