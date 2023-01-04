#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;
layout (location = 2) in float in_Shininess;
layout (location = 3) in vec3 in_Normal;

uniform mat4 viewShader;
uniform mat4 projectionShader;
uniform vec3 viewPosition;
uniform vec3 lightPosition;

out vec4 ex_Color;
out vec3 ex_FragPos;
out vec3 ex_Normal;
out vec3 ex_LightPosition;
out vec3 ex_ViewPosition;
out float ex_Shininess;
out float ex_Visibility;

const float density = 0.002f;
const float gradient = 5.0f;

void main() {
    mat4 camera = projectionShader * viewShader;
    vec4 position = camera * vec4(in_Position, 1.0);
    gl_Position = position;

    ex_Color = vec4(in_Color, 1.0f);
    ex_FragPos = vec3(gl_Position);
    ex_Normal = vec3(camera * vec4(in_Normal, 0.0));
    ex_LightPosition = vec3(camera * vec4(lightPosition, 1.0f));
    ex_ViewPosition = vec3(camera * vec4(viewPosition, 1.0f));
    ex_Shininess = in_Shininess;

    vec3 positionRelativeToCamera = ex_ViewPosition * position.xyz;
    float distance = length(positionRelativeToCamera);
    ex_Visibility = exp(-pow((distance * density), gradient));
    ex_Visibility = clamp(ex_Visibility, 0.0f, 1.0f);
}