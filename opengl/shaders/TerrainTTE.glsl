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
// uniform mat3 normalMat;

uniform sampler2D tex0; // heightmap

out EvalFrag
{
    vec3 normal;
    float height;
    vec3 fragPos;
    vec2 uv;
} evalFrag;

const float HEIGHT_MULTIPLIER = 3.f;

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

    evalFrag.height = texture(tex0, texCoord).y * HEIGHT_MULTIPLIER;

    // ----------------------------------------------------------------------
    // retrieve control point position coordinates
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // bilinearly interpolate position coordinate across patch
    //TODO: use TEXTURE COORDS NOT POINT COORDS
    // vec4 p0 = mix(t01, t00, u - 0.1f);
    // vec4 p1 = mix(t11, t10, u - 0.1f);
    // vec4 pL = mix(t0, t1, v);
    //
    // p0 = mix(p01, p00, u + 0.1f);
    // p1 = mix(p11, p10, u + 0.1f);
    // vec4 pR = mix(p0, p1, v);
    //
    // p0 = mix(p01, p00, u);
    // p1 = mix(p11, p10, u);
    // vec4 pU = mix(p0, p1, v + 0.1f);
    //
    // p0 = mix(p01, p00, u);
    // p1 = mix(p11, p10, u);
    // vec4 pD = mix(p0, p1, v - 0.1f);
    //
    //
    // p00.y = texture(tex0, pL.xy).y * HEIGHT_MULTIPLIER;
    // p01.y = texture(tex0, pR.xy).y * HEIGHT_MULTIPLIER;
    // p10.y = texture(tex0, pU.xy).y * HEIGHT_MULTIPLIER;
    // p11.y = texture(tex0, pD.xy).y * HEIGHT_MULTIPLIER;

    vec4 p0 = mix(p01, p00, u);
    vec4 p1 = mix(p11, p10, u);
    vec4 p = mix(p0, p1, v);

    p00.y = texture(tex0, t00).y * HEIGHT_MULTIPLIER;
    p01.y = texture(tex0, t01).y * HEIGHT_MULTIPLIER;
    p10.y = texture(tex0, t10).y * HEIGHT_MULTIPLIER;
    p11.y = texture(tex0, t11).y * HEIGHT_MULTIPLIER;

    // compute patch surface normal
    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    evalFrag.normal = normalize(cross(vVec.xyz, uVec.xyz));

    // displace point along normal
     p.y +=  evalFrag.height * 5;

    // ----------------------------------------------------------------------
    // output patch point position in clip space
    gl_Position = projection * view * model * p;
    evalFrag.fragPos = vec3(model * p);
}
