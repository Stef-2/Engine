#version 460 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexCoordinate;

layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 boneWeights;

out vec3 position;
out vec3 normal;
out vec2 uv;
out ivec4 boneids;
out vec4 boneweights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 boneTransformations[100];

void main()
{
    mat4 totalTransform = mat4(1.0f);
    vec4 totalPosition = vec4(1.0f);
    for(int i = 0 ; i < 4 ; i++)
    {
        if(boneIDs[i] == -1) 
            continue;
        if(boneIDs[i] >=100) 
        {
            totalPosition = vec4(vertexPosition,1.0f);
            break;
        }
        //vec4 localPosition = boneTransformations[boneIDs[i]] * vec4(vertexPosition, 1.0f);
        //totalPosition += localPosition * boneWeights[i];
        //vec3 localNormal = mat3(boneTransformations[boneIDs[i]]) * vertexNormal;
        totalPosition += boneTransformations[boneIDs[i]] * boneWeights[i] * vec4(vertexPosition, 1.0f);

        totalTransform += boneTransformations[boneIDs[i]] * boneWeights[i];
    }

    totalPosition -= vec4(vertexPosition, 1.0f) * 0.5;

    vec4 asdf = totalTransform * vec4(vertexPosition, 0.5f);

    asdf += vec4(vertexPosition, 0.5f);

    vec4 moved = vec4(vertexPosition, 1.0f) + totalPosition * 0.5;
    gl_Position =  projection * view * model * asdf;

    normal = vertexNormal;
	uv = vertexCoordinate;
    position = vertexPosition;
    boneids = boneIDs;
    boneweights = boneWeights;
}
