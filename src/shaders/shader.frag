#version 330 core

in vec4 ex_Color;
in vec3 ex_FragPos;
in vec3 ex_Normal;
in vec3 ex_LightPosition;
in vec3 ex_ViewPosition;
in float ex_Shininess;

uniform vec3 lightColor;
uniform vec3 skyColor;

out vec4 out_Color;

const float AMBIENT_STRENGTH = 0.7f;
const float SPECULAR_STRENGTH = 0.5f;

void main() {
    vec3 objectColor = vec3(ex_Color);

    // Ambient lighting
    vec3 ambientLight = AMBIENT_STRENGTH * skyColor;
    vec3 ambientTerm = mix(objectColor, ambientLight, 0.2f);

    // Diffuse lighting
    vec3 normal = normalize(ex_Normal);
    vec3 lightDirection = normalize(ex_LightPosition - ex_FragPos);
    float diffusionPercentage = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseTerm = diffusionPercentage * lightColor * objectColor;

    // Specular lighting
    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(ex_ViewPosition - ex_FragPos);
    vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), ex_Shininess);
    vec3 specularLight = specularStrength  * lightColor;
    vec3 specularTerm = spec * specularLight * objectColor;

    // Final color
    vec3 result = ambientTerm + diffuseTerm + specularTerm;
    out_Color = vec4(result, 1.0f);
}