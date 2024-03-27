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


// Calculates the shadow for the color calculation. Returns the inverse of the shadow
float calculateShadow(vec3 lightDir)
{
    // perform perspective divide. This is not needed for orthographic light matrix as it does not affect anything
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 1.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float lightDepth = texture(depthTex, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    //PCF - percentage closest filtering implementation
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthTex, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthTex, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
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

    float shadow = calculateShadow(lightDir);
    vec3 color = ((ambientVal * lightColor) + (shadow) + diffuse + specular) * objColor;
    fragColor = vec4(color, 1.f);
}
