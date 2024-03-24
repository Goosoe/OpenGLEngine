#version 330
out vec2 uv;

/*This creates a triangle that fits the window so we can use the UV freely*/
void main() {
    vec2 vertices[3] = vec2[3](vec2(-1.0, -1.0), vec2(3.0, -1.0), vec2(-1.0, 3.0));
    gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
    uv = 0.5 * gl_Position.xy + vec2(0.5);
}
