#version 460 core

in vec3 uv;

out vec4 fragColor;

uniform samplerCube cubeMap;

void main()
{
     fragColor = texture(cubeMap, uv);
}
