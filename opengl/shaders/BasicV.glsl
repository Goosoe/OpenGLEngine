#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

struct Camera
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

uniform Camera camera;

out vec3 normal;
out vec3 fragPos;

void main()
{
    gl_Position = camera.projection * camera.view * camera.model * vec4(aPos, 1.0f);
    normal = aNormal;
    fragPos = vec3(camera.model * vec4(aPos, 1.0));
}
