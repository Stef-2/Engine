#version 460 core

layout (location = 7) in mat4 instancedTransforms;

layout (binding = 0, std140) uniform mvpMatrices
{
	mat4 model;
	mat4 view;
	mat4 projection;
};

in vec4 color;

out vec4 fragColor;

void main()
{
	fragColor = instancedTransforms[1];
	fragColor = color;
	//fragColor = vec4(0.3f, 0.5f, 0.1f, 1.0f);
}
