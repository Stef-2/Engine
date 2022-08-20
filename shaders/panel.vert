#version 460 core

#define MAX_INSTANCES 128

layout (location = 0) in vec3 vertexPosition;
layout (location = 7) in mat4 instancedTransforms;
layout (location = 11) in vec3 instancedPositions;

layout (binding = 0, std140) uniform mvpMatrices
{
	mat4 model;
	mat4 view;
	mat4 projection;
};

out vec4 color;

void main()
{
	float x = vertexPosition.x;
	float y = vertexPosition.y;

	x *= instancedTransforms[0][2];
	y *= instancedTransforms[0][3];

	x += instancedTransforms[0][0];
	y += instancedTransforms[0][1];

	gl_Position = vec4(x, y, 0.0f, 0.0f);

	color = instancedTransforms[2];
}
