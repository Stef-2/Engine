#version 460 core

layout (location = 0) in vec3 vertexPosition;

layout (binding = 0, std140) uniform mvpMatrices
{
	mat4 model;
	mat4 view;
	mat4 projection;
};

in vec4 vertPosition;

out vec4 fragColor;

void main()
{
	fragColor = vec4(vec3(1.0f), 0.5f);
}