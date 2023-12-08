#version 430 core

in vec3 normal;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform float ambientVal;
uniform float specularVal;

out vec4 fragColor;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularVal * spec * lightColor;

    vec3 color = ((ambientVal * lightColor) + diffuse + specular) * objColor;
    //vec3 color = normal * 0.5 + 0.5;
    fragColor = vec4(color, 1.f);
}
