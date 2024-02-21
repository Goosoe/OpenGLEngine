#version 430 core

in vec3 normal;
in vec3 fragPos;
in float height;
in vec3 pos;
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
    //fragColor = vec4(pos, 1.f);
    fragColor = vec4(1.f);
    return;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularVal * spec * lightColor;

    vec3 lighting = ((ambientVal * lightColor) + diffuse + specular) * objColor;

    //todo: uniform this
    const float GRASS_H = -0.8f;
    const float ROCK_H = 0.5f;
    const float SNOW_H = 0.9f;

    //todo: improve this
     if(height < GRASS_H)  //grass
    {
        vec4 tex0Col = texture(tex0, texCoord);
        fragColor = tex0Col * vec4(lighting, 1.f);
    }
    else if (height < ROCK_H)  //grass/rock
    {
        float delta = ROCK_H - GRASS_H;
        float factor = (height - GRASS_H) / delta;
        vec4 tex0Col = texture(tex0, texCoord);
        vec4 tex1Col = texture(tex1, texCoord);
        fragColor = mix(tex0Col, tex1Col, factor) * vec4(lighting, 1.f);
    }
    else if (height < SNOW_H)  //rock/snow
    {
        float delta = SNOW_H - ROCK_H;
        float factor = (height - ROCK_H) / delta;
        vec4 tex1Col = texture(tex1, texCoord);
        vec4 tex2Col = texture(tex2, texCoord);
        fragColor = mix(tex1Col, tex2Col, factor) * vec4(lighting, 1.f);
    }
   else 
   {
       vec4 tex2Col = texture(tex2, texCoord);
       fragColor = tex2Col * vec4(lighting, 1.f);
   }
}
