//Tesselation Evaluation Shader for the Terrain
#version 430 core

layout (quads, fractional_odd_spacing, ccw) in;

in CtrlEval{
    vec2 texCoord;
} ctrlEval[];

uniform mat4 model;           // the model matrix
uniform mat4 view;            // the view matrix
uniform mat4 projection;      // the projection matrix
uniform float heightScale;

uniform sampler2D tex0; // heightmap

out EvalFrag
{
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
    vec2 t0 = mix(t01, t00, u);
    vec2 t1 = mix(t11, t10, u);
    vec2 texCoord = mix(t0, t1, v);
    evalFrag.uv = texCoord;

    // lookup texel at patch coordinate for height and scale + shift as desired

    evalFrag.height = texture(tex0, texCoord).r;// * heightScale;

    // ----------------------------------------------------------------------
    // retrieve control point position coordinates
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    //vertex position we are working on
    vec4 p0 = mix(p01, p00, u);
    vec4 p1 = mix(p11, p10, u);
    vec4 p = mix(p0, p1, v);
    p.y +=  evalFrag.height * heightScale;

    // ----------------------------------------------------------------------
    // output patch point position in clip space
    gl_Position = projection * view * model * p;
    evalFrag.fragPos = vec3(model * p);
}
