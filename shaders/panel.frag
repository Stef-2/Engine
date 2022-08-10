#version 460 core

layout (location = 0) in vec3 vertexPosition;

out vec4 fragColor;

void main()
{
	fragColor = vec4(vec3(1.0f), 0.5f);
}