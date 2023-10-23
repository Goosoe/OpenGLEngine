#version 430 core

out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
    //FragColor = texture(tex2, texCoord); //* vertexColor;
    FragColor = mix(texture(tex, texCoord), texture(tex2, texCoord), 0.2) * vertexColor;
}
