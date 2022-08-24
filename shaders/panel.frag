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
	flat vec4 edges;
	flat vec4 borderColor;
	flat float borderThickness;
	flat float screenWidth;
	flat float screenHeight;
	vec3 pos;
};

out vec4 fragColor;

bool BorderArea()
{
	float border = borderThickness;
	float ratio = screenWidth / screenHeight;
	float x = gl_FragCoord.x;// / screenWidth * 2 - 1.0f ;
	float y = gl_FragCoord.y;// / screenHeight * 2 - 1.0f;
	
	float mult = .5;
	float add = 1;

	float left = (edges[0] + add) * screenWidth * mult ;
	float right = (edges[1] + add) * screenWidth * mult ;
	float bottom = (edges[2] + add / ratio) * (screenHeight  * ratio) * mult ;
	float top = (edges[3] + add / ratio) * (screenHeight * ratio) * mult ;

	if (borderThickness > 0) vecs[0] = vec4(left, right, bottom, top);

	//y/=ratio;
	return x > left + border && x < right - border &&
			y > bottom + border && y < top - border;
};

void main()
{
	//if (BorderArea()) vecs[0] = vec4(gl_FragCoord.x / screenWidth, gl_FragCoord.y / screenHeight, 33, 44);
	vec4 result = (borderThickness > 0.0f && BorderArea()) ? borderColor : color;
	fragColor = result;
}
