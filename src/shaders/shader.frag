#version 330 core

in vec4 ex_Color;
in vec3 ex_FragPos;
in vec3 ex_Normal;
in vec3 ex_LightPosition;
in vec3 ex_ViewPosition;
in float ex_Shininess;
in float ex_Visibility;

uniform vec3 lightColor;
uniform vec3 skyColor;

out vec4 out_Color;

const float AMBIENT_STRENGTH = 0.4f;
const float SPECULAR_STRENGTH = 0.2f;

void main() {
    vec3 objectColor = vec3(ex_Color);

    // Ambient lighting
    vec3 ambientTerm = AMBIENT_STRENGTH * skyColor;

    // Diffuse lighting
    vec3 normal = normalize(ex_Normal);
    vec3 lightDirection = normalize(ex_LightPosition - ex_FragPos);
    float diffusionPercentage = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseTerm = diffusionPercentage * lightColor;

    // Specular lighting
    vec3 viewDirection = normalize(ex_ViewPosition - ex_FragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularPercentage = pow(max(dot(viewDirection, reflectDirection), 0.0), ex_Shininess);
    vec3 specularTerm = SPECULAR_STRENGTH * specularPercentage * lightColor;

    // Final color
    vec3 result = (ambientTerm + diffuseTerm + specularTerm) * objectColor;
    out_Color = vec4(result, 1.0f);

    out_Color = mix(vec4(skyColor, 1.0f), out_Color, ex_Visibility);
}