#version 460 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexBitangent;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec2 vertexCoordinate;

layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 boneWeights;

layout (location = 7) in mat4 instancedTransform;
layout (location = 11) in vec3 instancedPosition;

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

out vec3 position;

void main()
{   
    mat4 matrix = (instancedTransform[0][0] != 0.0f) ? instancedTransform : mat4(1.0f);
    vec3 position = (instancedPosition != vec3(0.0f)) ? instancedPosition + vertexPosition : vertexPosition;

    mats[3] = matrix;
    vecs[3] = vec4(position, 77);

    gl_Position = projection * view * matrix * vec4(position, 1.0f);
    //position = normalize(instancedPosition);
}
