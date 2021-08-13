#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 vertexCoordinate;

attribute ivec3 boneID;
attribute vec3 boneWeight;

varying vec3 normal;
varying vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
    normal = vertexNormal;
    uv = vertexCoordinate;
}
