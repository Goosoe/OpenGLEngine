#version 430 core

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

//in vec4 vertexColor;
//in vec2 texCoord;

//uniform sampler2D tex;
//uniform sampler2D tex2;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform float ambientVal;
uniform float specularVal;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularVal* spec * lightColor;

    vec3 color = ((ambientVal * lightColor) + diffuse + specular) * objColor;
    FragColor = vec4(color, 1.f);
    //FragColor = texture(tex2, texCoord); //* vertexColor;
    //FragColor = mix(texture(tex, texCoord), texture(tex2, texCoord), 0.6) * vertexColor;
}
