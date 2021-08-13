#version 120

varying vec3 normal;
varying vec3 uv;

uniform samplerCube cubeMap;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 color = textureCube(cubeMap, uv);
    gl_FragColor = color;
}
