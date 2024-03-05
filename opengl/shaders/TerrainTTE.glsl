//Tesselation Evaluation Shader for the Terrain
#version 430 core

layout (quads, fractional_odd_spacing, ccw) in;

// received from Tessellation Control Shader - all texture coordinates for the patch vertices
in CtrlEval{
    vec2 texCoord;
    vec3 normal;
} ctrlEval[];

uniform mat4 model;           // the model matrix
uniform mat4 view;            // the view matrix
uniform mat4 projection;      // the projection matrix
uniform mat3 normalMat;

uniform sampler2D tex0; // heightmap

out EvalFrag
{
    vec3 normal;
    float height;
    vec3 fragPos;
    vec2 uv;
} evalFrag;

void main()
{
    // get patch coordinate
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // ----------------------------------------------------------------------
    // retrieve control point texture coordinates
    vec2 t00 = ctrlEval[0].texCoord;
    vec2 t01 = ctrlEval[1].texCoord;
    vec2 t10 = ctrlEval[2].texCoord;
    vec2 t11 = ctrlEval[3].texCoord;

    // bilinearly interpolate texture coordinate across patch
    vec2 t0 = mix(t00, t01, v);
    vec2 t1 = mix(t10, t11, v);
    vec2 texCoord = mix(t0, t1, u);
    evalFrag.uv = texCoord;

    // lookup texel at patch coordinate for height and scale + shift as desired
    evalFrag.height = texture(tex0, texCoord).y * 3;

    // ----------------------------------------------------------------------
    // retrieve control point position coordinates
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // compute patch surface normal
    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;

    //average the normal out
    evalFrag.normal = normalize(normalMat * (ctrlEval[0].normal + ctrlEval[1].normal + ctrlEval[2].normal + ctrlEval[3].normal)/ 4);

    // bilinearly interpolate position coordinate across patch
    vec4 p0 = mix(p00, p01, u);
    vec4 p1 = mix(p10, p11, u);
    vec4 p = mix(p0, p1, v);

    // displace point along normal
     p += vec4(evalFrag.normal * evalFrag.height, 1.f);

    // ----------------------------------------------------------------------
    // output patch point position in clip space
    gl_Position = projection * view * model * p;
    evalFrag.fragPos = vec3(model * p);
}
