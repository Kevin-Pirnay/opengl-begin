#version 330 core

in vec3 pos_input;
in vec3 col_input;
in vec2 texCoords_input;
out vec4 color_input;
out vec2 texCoords;

uniform mat4 rotation_z;
uniform mat4 rotation_y;
uniform mat4 rotation_x;
uniform mat4 translation;

void main()
{
    gl_Position = rotation_z * rotation_y * rotation_x * vec4(pos_input, 1.0);
    color_input = vec4(col_input, 1.0);
    texCoords = texCoords_input;
}