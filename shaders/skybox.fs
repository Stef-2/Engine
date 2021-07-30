#version 120

varying vec3 texCoords;

uniform samplerCube cubeMap;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 color = textureCube(cubeMap, texCoords);
    gl_FragColor = color;
}
