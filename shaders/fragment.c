#version 330 core

in vec4 color_input;
in vec2 texCoords;
out vec4 color;
uniform sampler2D texture_sampler;

void main()
{
    color = texture(texture_sampler, texCoords) * color_input;
}