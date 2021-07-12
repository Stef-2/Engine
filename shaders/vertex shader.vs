#version 120

attribute vec3 vertPos;
attribute vec2 vertCoord;

varying vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vertPos, 1.0f);
    texCoord = vertCoord;
}
