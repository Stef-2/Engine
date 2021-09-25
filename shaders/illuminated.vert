#version 460 core

#define MAX_NUM_POINT_LIGHTS_PER_VERTEX 8
#define MAX_NUM_SPOT_LIGHTS_PER_VERTEX 4
#define MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX 4

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexBitangent;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec2 vertexCoordinate;

layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 boneWeights;

struct PointLight {
    vec4 position;
    vec4 color;
    float intensity;
};

struct SpotLight {
    vec4 position;
    vec4 rotation;
    vec4 color;
    float intensity;
    float angle;
    float sharpness;
};

struct DirectionalLight {
    vec4 position;
    vec4 rotation;
    vec4 color;
    float intensity;
};

struct AmbientLight {
    vec4 position;
    vec4 color;
    float intensity;
};

layout (binding = 0, std140) uniform mvpMatrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

layout (binding = 1, std430) buffer PointLights {
  PointLight pointLights[];
};

layout (binding = 2, std430) buffer SpotLights {
  SpotLight spotLights[];
};

layout (binding = 3, std430) buffer DirectionalLights {
  DirectionalLight directionalLights[];
};

layout (binding = 4, std430) buffer AmbientLights {
  AmbientLight ambientLights[];
};

uniform mat4 boneTransformations[100];

out vertOutput
{
    vec3 position;
    mat3 TBN;
    vec2 uv;
    
    unsigned int numPointLights;
    unsigned int usedPointLightIndices[MAX_NUM_POINT_LIGHTS_PER_VERTEX];
    vec4 tangentSpacePointLights[MAX_NUM_POINT_LIGHTS_PER_VERTEX];

    unsigned int numSpotLights;
    unsigned int usedSpotLightIndices[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];
    vec4 tangentSpaceSpotLights[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];

    unsigned int numAmbientLights;
    unsigned int usedAmbientLightIndices[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
    vec4 tangentSpaceAmbientLights[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
};

out vec2 asd;

// calculate the final transformation for a bone
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

// inverse Tangent, Bitangent, Normal matrix for lighting calculations in fragment shader
mat3 calculateInverseTBN(vec3 transNormal)
{
    vec3 tangent = normalize(vec3(model * vec4(vertexTangent, 0.0)));
    vec3 bitangent = normalize(vec3(model * vec4(vertexBitangent, 0.0)));
    vec3 normal = normalize(vec3(model * vec4(transNormal, 0.0)));

    return transpose(mat3(tangent, bitangent, normal));
}

// go through all point lights and check for ones whose light will have a significant contribution to the vertex, record its index into the light stack
void ProcessPointLights(inout unsigned int numPointLights, inout unsigned int usedPointLightIndices[MAX_NUM_POINT_LIGHTS_PER_VERTEX], in vec3 animatedPos)
{
    numPointLights = 0;

    for (unsigned int i = 0; i < pointLights.length(); i++)
    {
        // point and spot lights use inverse square decay for their intensity, so at distance 10 they will reach 1% of their initial value
        if (distance(animatedPos, pointLights[i].position.xyz) <= 10.0f) {
            usedPointLightIndices[numPointLights] = i;
            numPointLights += 1;
        }

        if (numPointLights == MAX_NUM_POINT_LIGHTS_PER_VERTEX)
            break;
    }
}

// go through all spot lights and check for ones whose light will have a significant contribution to the vertex, record its index into the light stack
void ProcessSpotLights(inout unsigned int numSpotLights, inout unsigned int usedSpotLightIndices[MAX_NUM_SPOT_LIGHTS_PER_VERTEX], in vec3 animatedPos)
{
    numSpotLights = 0;

    for (unsigned int i = 0; i < spotLights.length(); i++)
    {
        // point and spot lights use inverse square decay for their intensity, so at distance 10 they will reach 1% of their initial value
        if (distance(animatedPos, spotLights[i].position.xyz) <= 10.0f) {
            usedPointLightIndices[numSpotLights] = i;
            numSpotLights += 1;
        }

        if (numSpotLights == MAX_NUM_SPOT_LIGHTS_PER_VERTEX)
            break;
    }
}
// go through all ambient lights and check for ones whose light will have a significant contribution to the vertex, record its index into the light stack
void ProcessAmbientLights(inout unsigned int numAmbientLights, inout unsigned int usedAmbientLightIndices[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX], in vec3 animatedPos)
{
    numAmbientLights = 0;

    for (unsigned int i = 0; i < ambientLights.length(); i++)
    {
        // ambient lights use a linear intensity falloff, so we can cull them past their intensity radius
        if (distance(animatedPos, ambientLights[i].position.xyz) <= ambientLights[i].intensity) {
            usedAmbientLightIndices[numAmbientLights] = i;
            numAmbientLights += 1;
        }

        if (numAmbientLights == MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX)
            break;
    }
}

void main()
{
    mat4 animatedTransform = BoneTransformation(boneIDs, boneWeights, boneTransformations);
    vec4 animatedPosition = animatedTransform * vec4(vertexPosition, 1.0f);
    vec3 animatedNormal = (animatedTransform * vec4(vertexNormal, 0.0f)).xyz;

    gl_Position =  projection * view * model * animatedPosition;

    position = animatedPosition.xyz;
    TBN = calculateInverseTBN(animatedNormal);
    uv = vertexCoordinate;

    //ProcessPointLights(numPointLights, usedPointLightIndices, animatedPosition.xyz);
    //ProcessSpotLights(numSpotLights, usedSpotLightIndices, animatedPosition.xyz);
    //ProcessAmbientLights(numAmbientLights, usedAmbientLightIndices, animatedPosition.xyz);
}
