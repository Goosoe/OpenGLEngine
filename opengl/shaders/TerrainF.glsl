#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec3 worldPos;
in vec2 texCoord;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform float ambientVal;
uniform float specularVal;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

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

    vec3 lighting = ((ambientVal * lightColor) + diffuse + specular) * objColor;
    //fragColor = vec4(lighting, 1.f);
    vec4 tex0Col = texture(tex0, texCoord);
    vec4 tex1Col = texture(tex1, texCoord);
    vec4 tex2Col = texture(tex2, texCoord);
     if(worldPos.y < 0.6f)  //grass
        fragColor = tex0Col * vec4(lighting, 1.f);
    else if (worldPos.y < 2.2f)  //snow
        fragColor = tex1Col * vec4(lighting, 1.f);
    else    //snow
        fragColor = tex2Col * vec4(lighting, 1.f);
    //fragColor = texture(tex0, texCoord) * vec4(lighting, 1.f);
}
