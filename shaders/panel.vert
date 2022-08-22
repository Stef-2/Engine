#version 460 core

#define MAX_INSTANCES 128

layout (location = 0) in vec3 vertexPosition;
layout (location = 7) in mat4 instancedTransforms;
layout (location = 11) in vec3 instancedPositions;

layout (binding = 69, std430) buffer Debug {
	mat4 mats[4];
	vec4 vecs[4];
	float floats[3];

	// set this to a non zero value to trigger a breakpoint
	float breakPoint;
};

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

	floats[0] = vertexPosition.x;
	floats[1] = vertexPosition.y;

	gl_Position = vec4(x, y, instancedTransforms[1][1], 1.0f);

	
	color = instancedTransforms[1];
}
