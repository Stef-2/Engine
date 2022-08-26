#version 460 core

layout (location = 7) in mat4 instancedTransforms;

layout (binding = 0, std140) uniform mvpMatrices
{
	mat4 model;
	mat4 view;
	mat4 projection;
};

layout (binding = 69, std430) buffer Debug {
	mat4 mats[4];
	vec4 vecs[4];
	float floats[3];

	// set this to a non zero value to trigger a breakpoint
	float breakPoint;
};

in VertexOutput
{
	flat vec4 color;
	flat float edges[4];
	flat vec4 borderColor;
	flat float borderThickness;
	flat float screenWidth;
	flat float screenHeight;
};

out vec4 fragColor;

bool BorderArea()
{
	float x = gl_FragCoord.x;
	float y = gl_FragCoord.y;
	
	float mult = 0.5;
	float add = 0.5;

	float left = (edges[0] * 0.5f + 0.5f);
	float right = (edges[1] * 0.5f + 0.5f);
	float bottom = (edges[2] * 0.5f + 0.5f);
	float top = (edges[3] * 0.5f + 0.5f);
	
	return x > left * screenWidth + borderThickness && x < right * screenWidth - borderThickness &&
			y > bottom * screenHeight + borderThickness && y < top * screenHeight - borderThickness;
};

void main()
{
	fragColor = (borderThickness > 0.0f && BorderArea()) ? borderColor : color;
}
