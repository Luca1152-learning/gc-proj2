#version 330 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec4 in_Color;

out vec4 ex_Color;
uniform mat4 matrix;

void main() {
    gl_Position = matrix * in_Position;
    ex_Color = in_Color;
}