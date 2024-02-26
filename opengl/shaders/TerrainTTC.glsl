// Tesselation Evaluation Shader for the Terrain
#version 410 core

layout (vertices=4) out;

// varying input from vertex shader
in vec2 TexCoord[];
in vec3 normal[];

// varying output to evaluation shader
out vec2 TextureCoord[];
out vec3 aNormal[];
void main()
{
    // invocation zero controls tessellation levels for the entire patch
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 16;
        gl_TessLevelOuter[1] = 16;
        gl_TessLevelOuter[2] = 16;
        gl_TessLevelOuter[3] = 16;

        gl_TessLevelInner[0] = 16;
        gl_TessLevelInner[1] = 16;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    TextureCoord[gl_InvocationID] = TexCoord[gl_InvocationID];
    aNormal[gl_InvocationID] = normal[gl_InvocationID];
}
