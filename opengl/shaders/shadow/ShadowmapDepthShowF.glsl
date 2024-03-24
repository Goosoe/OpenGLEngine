#version 430

in vec2 uv;

out vec4 fragColor;

uniform sampler2D depthTex;

void main()
{ 
    fragColor = vec4(vec3(texture(depthTex, uv).r), 1.f);
    // fragColor = vec4(1, 0, 0, 1);
    // fragColor = vec4(uv, 0, 1);
}
