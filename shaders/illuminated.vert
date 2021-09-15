#version 460 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexCoordinate;

layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 boneWeights;

out vertOutput
{
    vec3 normal;
    vec2 uv;
};

layout (binding = 0, std140) uniform Matrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

layout (binding = 1, std140) uniform AmbientLights
{
    vec3 color;
    float intensity;
} ambientLights[];

layout (binding = 2, std140) uniform PointLighs
{
    vec3 position;
    vec3 color;
    float intensity;
} pointLights[];

layout (binding = 3, std140) uniform SpotLights
{
    vec3 position;
    vec3 orientation;
    vec3 color;
    float intensity;
} spotLights[];

layout (binding = 4, std140) uniform DirectionalLights
{
    vec3 orientation;
    vec3 color;
    float intensity;
} directionalLights[];

uniform mat4 boneTransformations[100];

struct PointLight
{
    vec3 position;
    vec3 color;
    float intensity;
};

vec4 BoneTransformation(vec3 vertexPosition, ivec4 boneIDs, vec4 boneWeights, mat4 boneTransformations[100])
{
    vec4 position = vec4(0.0f);

    for(int i = 0 ; i < 4 ; i++)
    {
        if(boneIDs[i] == -1) 
            continue;

        position += boneTransformations[boneIDs[i]] * boneWeights[i] * vec4(vertexPosition, 1.0f);
    }
    return position;
}

void main()
{

    gl_Position =  projection * view * model * BoneTransformation(vertexPosition, boneIDs, boneWeights, boneTransformations);
    normal = vertexNormal;
	uv = vertexCoordinate;
}
