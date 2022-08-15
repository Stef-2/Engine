#version 460 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexBitangent;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec2 vertexCoordinate;

layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 boneWeights;

layout (location = 7) in mat4 instancedTransform;

layout (binding = 0, std140) uniform mvpMatrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main()
{
    gl_Position = projection * view * instancedTransform * vec4(vertexPosition, 1.0f);
}
