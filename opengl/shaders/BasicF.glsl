#version 430 core

out vec4 FragColor;

//in vec4 vertexColor;
//in vec2 texCoord;

//uniform sampler2D tex;
//uniform sampler2D tex2;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform float ambient;

void main()
{
    vec3 color = ambient * lightColor * objColor;
    FragColor = vec4(color, 1.f);
    //FragColor = texture(tex2, texCoord); //* vertexColor;
    //FragColor = mix(texture(tex, texCoord), texture(tex2, texCoord), 0.6) * vertexColor;
}
