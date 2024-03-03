#version 430 core

in EvalFrag
{
    vec3 normal;
    float height;
    vec3 fragPos;
    vec2 uv;
} evalFrag;

// in vec3 normal;
// in vec3 fragPos;
// // in vec3 worldPos;
// in vec2 uv;
// in float height;
// in vec2 texTest;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform float ambientVal;
uniform float specularVal;

// uniform sampler2D tex0; // heightmap
uniform sampler2D tex1; // grass
uniform sampler2D tex2; // rock
uniform sampler2D tex3; // snow

out vec4 fragColor;

void main()
{
    vec3 lightDir = normalize(lightPos - evalFrag.fragPos);

    float diff = max(dot(evalFrag.normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(cameraPos - evalFrag.fragPos);
    vec3 reflectDir = reflect(-lightDir, evalFrag.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularVal * spec * lightColor;

    vec3 lighting = ((ambientVal * lightColor) + diffuse + specular) * objColor;

    //todo: uniform this
    const float GRASS_H = 0.2f;
    const float ROCK_H = 0.4f;
    const float SNOW_H = 0.9f * 2;

    //todo: improve this
     if(evalFrag.height < GRASS_H)  //grass
     {
         vec4 tex1Col = texture(tex1, 16 * evalFrag.uv);
         fragColor = tex1Col * vec4(lighting, 1.f);
     }
     else if (evalFrag.height < ROCK_H)  //grass/rock
     {
         float delta = ROCK_H - GRASS_H;
         float factor = (evalFrag.height - GRASS_H) / delta;
         vec4 tex1Col = texture(tex1, 16 * evalFrag.uv);
         vec4 tex2Col = texture(tex2, 16 * evalFrag.uv);
         fragColor = mix(tex1Col, tex2Col, factor) * vec4(lighting, 1.f);
     }
     else if (evalFrag.height < SNOW_H)  //rock/snow
     {
         float delta = SNOW_H - ROCK_H;
         float factor = (evalFrag.height - ROCK_H) / delta;
         vec4 tex2Col = texture(tex2, 16 * evalFrag.uv);
         vec4 tex3Col = texture(tex3, 16 * evalFrag.uv);
         fragColor = mix(tex2Col, tex3Col, factor) * vec4(lighting, 1.f);
     }
     else
     {
         vec4 tex3Col = texture(tex3, 16 * evalFrag.uv);
         fragColor = tex3Col * vec4(lighting, 1.f);
     }
}
