#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 2) in vec4 matModelCol0;
layout (location = 3) in vec4 matModelCol1;
layout (location = 4) in vec4 matModelCol2;
layout (location = 5) in vec4 matModelCol3;

layout (location = 6) in vec4 color;

uniform mat4 matProjection;
uniform mat4 matView;

out vec4 vColor;

void main() {
    mat4 matModel = mat4(matModelCol0, matModelCol1, matModelCol2, matModelCol3);
    gl_Position = matProjection * matView * matModel * vec4(position, 1.0f);
    vColor = color;
}
