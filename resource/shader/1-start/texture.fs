#version 460 core

in vec3  ourColor;
in vec2  texCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float     mixValue;

void main()
{
    FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(1 - texCoord.x, texCoord.y)), mixValue);
}