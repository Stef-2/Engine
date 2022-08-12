#version 460 core

#define MAX_INSTANCES 128

layout (location = 0) in vec3 vertexPosition;
layout (location = 7) in mat4 instancedTransforms;

layout (binding = 0, std140) uniform mvpMatrices

{
	mat4 model;
	mat4 view;
	mat4 projection;
};

out vec4 vertPosition;

void main()
{
	gl_Position = vec4(vertexPosition, 1.0f);
}