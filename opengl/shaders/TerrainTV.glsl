#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VertControl
{
    vec2 texCoord;
    vec3 normal;
} vertControl;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertControl.normal = aNormal;
    vertControl.texCoord = aTexCoord;
}
