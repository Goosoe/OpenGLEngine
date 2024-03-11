#version 430 core

in EvalFrag
{
    float height;
    vec3 fragPos;
    vec2 uv;
} evalFrag;

uniform mat4 model;
uniform mat3 inverseModel;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec2 texelSize;
uniform float ambientVal;
uniform float specularVal;
uniform float heightScale;

uniform sampler2D tex0; // heightmap
uniform sampler2D tex1; // grass
uniform sampler2D tex2; // rock
uniform sampler2D tex3; // snow

out vec4 fragColor;

void main()
{
    float height = evalFrag.height * heightScale;
    vec3 x = dFdx(evalFrag.fragPos); 
    vec3 y = dFdy(evalFrag.fragPos);
    vec3 normal = inverse(mat3(model)) * normalize(cross(x, y));

    vec3 lightDir = normalize(lightPos - evalFrag.fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(cameraPos - evalFrag.fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularVal * spec * lightColor;

    vec3 lighting = ((ambientVal * lightColor) + diffuse + specular) * objColor;

    //todo: uniform this
    const float GRASS_H = 0.2f * heightScale;
    const float ROCK_H = 0.4f * heightScale;
    const float SNOW_H = 0.9f * heightScale;

    //todo: improve this 16 IS A MAGIC NUMBER
     if(height < GRASS_H)  //grass
     {
         vec4 tex1Col = texture(tex1, 16 * evalFrag.uv);
         fragColor = tex1Col * vec4(lighting, 1.f);
     }
     else if (height < ROCK_H)  //grass/rock
     {
         float delta = ROCK_H - GRASS_H;
         float factor = (height - GRASS_H) / delta;
         vec4 tex1Col = texture(tex1, 16 * evalFrag.uv);
         vec4 tex2Col = texture(tex2, 16 * evalFrag.uv);
         fragColor = mix(tex1Col, tex2Col, factor) * vec4(lighting, 1.f);
     }
     else if (height < SNOW_H)  //rock/snow
     {
         float delta = SNOW_H - ROCK_H;
         float factor = (height - ROCK_H) / delta;
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
