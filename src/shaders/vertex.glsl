#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 charPosition;
void main() {
    gl_Position = vec4(aPos + charPosition, 0.0, 1.0);
    gl_PointSize = 5.0;
}
