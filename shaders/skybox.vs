#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 vertexCoordinate;

varying vec3 normal;
varying vec3 uv;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    uv = vertexPosition;
    vec4 position = projection * view *  vec4(vertexPosition, 1.0f) + vec4(vertexCoordinate.x, vertexCoordinate.y, 0.0f, 0.0f) * 0;
    gl_Position = position.xyww;
    normal = vertexNormal;
}
