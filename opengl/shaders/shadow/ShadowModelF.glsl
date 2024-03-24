#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLightSpace;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform float ambientVal;
uniform float specularVal;
uniform sampler2D depthTex;

out vec4 fragColor;

const float bias = 0.005;

float calculateShadow()
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthTex, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
    return 1.f - shadow;
}

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

    float shadow = calculateShadow();
    vec3 color = ((ambientVal * lightColor) + (shadow) + diffuse + specular) * objColor;
    fragColor = vec4(color, 1.f);
}
