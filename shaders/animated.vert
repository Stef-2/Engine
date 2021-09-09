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

uniform mat4 boneTransforms[];

void main()
{
    vec4 finalPosition = vec4(0.0f);
    vec3 finalNormal = vec3(0.0f);

    for (int i = 0; i < 4; i++)
    {
        finalPosition += boneTransforms[boneIDs[i]] * vec4(vertexPosition, 1.0f) * boneWeights[i];
        finalNormal += mat3(boneTransforms[boneIDs[i]]) * vertexNormal;
    }

    gl_Position = projection * view * model * finalPosition;
    normal = finalNormal;
    uv = vertexCoordinate;
}
