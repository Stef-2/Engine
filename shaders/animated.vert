#version 460 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexCoordinate;

layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 boneWeights;

out vec3 normal;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 boneTransformations[100];

void main()
{
    vec4 position = vec4(0.0f);

    for(int i = 0 ; i < 4 ; i++)
    {
        if(boneIDs[i] == -1) 
            continue;

        position += boneTransformations[boneIDs[i]] * boneWeights[i] * vec4(vertexPosition, 1.0f);
    }

    gl_Position =  projection * view * model * position;
    normal = vertexNormal;
	uv = vertexCoordinate;
}
