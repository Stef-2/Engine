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

out VertexOutput
{
	flat vec4 color;
	flat vec4 edges;
	flat vec4 borderColor;
	flat float borderThickness;
	flat float screenWidth;
	flat float screenHeight;
	vec3 pos;
};

void main()
{
	float depth = instancedTransforms[2][0];
	float x = vertexPosition.x;
	float y = vertexPosition.y;

	x *= instancedTransforms[0][2];
	y *= instancedTransforms[0][3];

	x += instancedTransforms[0][0];
	y += instancedTransforms[0][1];

	gl_Position = vec4(x, y, depth, 1.0f);
	
	color = instancedTransforms[1];
	borderColor = instancedTransforms[3];

	screenWidth = instancedTransforms[2][2];
	screenHeight = instancedTransforms[2][3];

	borderThickness =  instancedTransforms[2][1];

	// left, right, bottom, top
	edges[0] = instancedTransforms[0][0] + instancedTransforms[0][2] * -0.5f;
	edges[1] = instancedTransforms[0][0] + instancedTransforms[0][2] * 0.5f;
	edges[2] = instancedTransforms[0][1] + instancedTransforms[0][3] * -0.5f;
	edges[3] = instancedTransforms[0][1] + instancedTransforms[0][3] * 0.5f;
	pos = gl_Position.xyz;
}
