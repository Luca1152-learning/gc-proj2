#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec4 in_Color;

uniform mat4 viewShader;
uniform mat4 projectionShader;

out vec4 ex_Color;

void main() {
    gl_Position = projectionShader * viewShader * vec4(in_Position, 1.0);
    ex_Color = in_Color;
}