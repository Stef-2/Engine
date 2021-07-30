#version 120

attribute vec3 vertPos;
attribute vec2 vertCoord;

varying vec3 texCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    texCoords = vertPos;
    vec4 position = projection * view *  vec4(vertPos, 1.0f) + vec4(vertCoord.x, vertCoord.y, 0.0f, 0.0f) * 0;
    gl_Position = position.xyww;
    //gl_Position = projection * view *  vec4(vertPos, 1.0f) + vec4(vertCoord.x, vertCoord.y, 0.0f, 0.0f) * 0;
}
