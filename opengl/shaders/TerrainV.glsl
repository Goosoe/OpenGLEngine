#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//to avoid deformations of the normals during scaling or modifications during a translation
uniform mat3 normalMat;

out vec3 normal;
out vec3 fragPos;
out vec3 worldPos;
out vec2 texCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    normal = normalMat * aNormal;
    texCoord = aTexCoord;
    worldPos = aPos;
    fragPos = vec3(model * vec4(aPos, 1.0f));
}
