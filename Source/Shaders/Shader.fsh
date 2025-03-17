#version 430

out vec4 FragColor;
in vec4 normalOut;

void main() {
    FragColor = vec4(0.5*normalOut.y+0.25, 0.5*normalOut.y+0.25, 0.5*normalOut.y+0.25, 1.0);
}