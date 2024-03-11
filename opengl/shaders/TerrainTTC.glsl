// Tesselation Control Shader for the Terrain
#version 430 core

layout(vertices=4) out;

in VertControl{
    vec2 texCoord;
} vertControl[];

uniform mat4 model;
uniform mat4 view;

uniform float fovCos;
const int outOfFrustumTessLevel = 0;

out CtrlEval
{
    out vec2 texCoord;
} ctrlEval[];

void main()
{
    // invocation zero controls tessellation levels for the entire patch
    if (gl_InvocationID == 0)
    {
        // define constants to control tessellation parameters
        // set these as desired for your world scale
        const int MIN_TESS_LEVEL = 8;
        const int MAX_TESS_LEVEL = 32;
        const float MIN_DISTANCE = 20;
        const float MAX_DISTANCE = 100;

        // ----------------------------------------------------------------------
        // transform each vertex into eye space
        vec4 eyeSpacePos00 = view * model * gl_in[0].gl_Position;
        vec4 eyeSpacePos01 = view * model * gl_in[1].gl_Position;
        vec4 eyeSpacePos10 = view * model * gl_in[2].gl_Position;
        vec4 eyeSpacePos11 = view * model * gl_in[3].gl_Position;

        // ----------------------------------------------------------------------
        //  "distance" from camera scaled between 0 and 1
        float distance00 = clamp((length(eyeSpacePos00.xyz) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance01 = clamp((length(eyeSpacePos01.xyz) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance10 = clamp((length(eyeSpacePos10.xyz) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance11 = clamp((length(eyeSpacePos11.xyz) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        // ----------------------------------------------------------------------
        // interpolate edge tessellation level based on closer vertex
        float tessLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11));
        float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01));
        float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00));
        float tessLevel3 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10));

        // calculate cos of the angle between the camera and -z(0.0, 0.0, -1.0)
        float cosAngle00 = dot(normalize(eyeSpacePos00.xyz), vec3(0.0, 0.0, -1.0));
        float cosAngle01 = dot(normalize(eyeSpacePos01.xyz), vec3(0.0, 0.0, -1.0));
        float cosAngle10 = dot(normalize(eyeSpacePos10.xyz), vec3(0.0, 0.0, -1.0));
        float cosAngle11 = dot(normalize(eyeSpacePos11.xyz), vec3(0.0, 0.0, -1.0));


        // Outside the view frustum
        if (cosAngle00 <= fovCos &&
            cosAngle01 <= fovCos &&
            cosAngle10 <= fovCos &&
            cosAngle11 <= fovCos)
        {
            tessLevel0 = outOfFrustumTessLevel;
            tessLevel1 = outOfFrustumTessLevel;
            tessLevel2 = outOfFrustumTessLevel;
            tessLevel3 = outOfFrustumTessLevel;
        }
        // ----------------------------------------------------------------------
        // set the corresponding outer edge tessellation levels
        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        // ----------------------------------------------------------------------
        // set the inner tessellation levels to the max of the two parallel edges
        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    ctrlEval[gl_InvocationID].texCoord = vertControl[gl_InvocationID].texCoord;
}
