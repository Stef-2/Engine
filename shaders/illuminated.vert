#version 460 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexBitangent;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec2 vertexCoordinate;

layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 boneWeights;

out vertOutput
{
    vec3 position;
    vec3 normal;
    vec3 bitangent;
    vec3 tangent;
    vec2 uv;
};

layout (binding = 0, std140) uniform mvpMatrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

uniform mat4 boneTransformations[100];


mat4 BoneTransformation(ivec4 boneIDs, vec4 boneWeights, mat4 boneTransformations[100])
{
    mat4 transform = mat4(0.0f);

    for(int i = 0 ; i < 4 ; i++)
    {
        if(boneIDs[i] == -1) 
            continue;

        transform += boneTransformations[boneIDs[i]] * boneWeights[i];
    }

    return transform;
}

void main()
{
    mat4 animatedTransform = BoneTransformation(boneIDs, boneWeights, boneTransformations);
    vec4 animatedPosition = animatedTransform * vec4(vertexPosition, 1.0f);
    vec3 animatedNormal = (animatedTransform * vec4(vertexNormal, 0.0f)).xyz;

    gl_Position =  projection * view * model * animatedPosition;

    position = animatedPosition.xyz;
    normal = animatedNormal;
    bitangent = vertexBitangent;
    tangent = vertexTangent;
	uv = vertexCoordinate;
}
