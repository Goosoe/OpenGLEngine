#version 430 core

layout (location = 0) in vec3 aPos;

struct Camera
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

uniform Camera camera;

void main()
{
    gl_Position = camera.projection * camera.view * camera.model * vec4(aPos, 1.0f);
}
