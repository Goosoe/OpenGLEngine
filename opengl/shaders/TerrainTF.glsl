#version 430 core

in vec3 normal;
in vec3 fragPos;
// in vec3 worldPos;
in vec2 uv;
in float height;
//in vec2 texTest;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform float ambientVal;
uniform float specularVal;

uniform sampler2D tex0; // heightmap
uniform sampler2D tex1; // grass
uniform sampler2D tex2; // rock
uniform sampler2D tex3; // snow

out vec4 fragColor;

void main()
{
    // float h = (height + 16)/64.0f;
    // fragColor = vec4(texTest , 0.f, 1.f);
    // fragColor = vec4(1.f);
    // return;
    //fragColor = vec4(h, h, h, 1.0);

    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
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
        vec4 tex0Col = texture(tex0, uv);
        fragColor = tex0Col * vec4(lighting, 1.f);
    }
    else if (height < ROCK_H)  //grass/rock
    {
        float delta = ROCK_H - GRASS_H;
        float factor = (height - GRASS_H) / delta;
        vec4 tex0Col = texture(tex0, uv);
        vec4 tex1Col = texture(tex1, uv);
        fragColor = mix(tex0Col, tex1Col, factor) * vec4(lighting, 1.f);
    }
    else if (height < SNOW_H)  //rock/snow
    {
        float delta = SNOW_H - ROCK_H;
        float factor = (height - ROCK_H) / delta;
        vec4 tex1Col = texture(tex1, uv);
        vec4 tex2Col = texture(tex2, uv);
        fragColor = mix(tex1Col, tex2Col, factor) * vec4(lighting, 1.f);
    }
    else
    {
        vec4 tex2Col = texture(tex2, uv);
        fragColor = tex2Col * vec4(lighting, 1.f);
    }
}
