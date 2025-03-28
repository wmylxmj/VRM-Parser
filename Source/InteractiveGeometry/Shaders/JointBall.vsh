#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 2) in vec4 transformCol0;
layout (location = 3) in vec4 transformCol1;
layout (location = 4) in vec4 transformCol2;
layout (location = 5) in vec4 transformCol3;

layout (location = 6) in vec4 color;

uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

out vec4 vColor;

void main() {
    mat4 transformation = mat4(transformCol0, transformCol1, transformCol2, transformCol3);
    gl_Position = matProjection * matView * matModel * transformation * vec4(position, 1.0f);
    vColor = color;
}
