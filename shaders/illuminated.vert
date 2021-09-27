#version 460 core

#define MAX_NUM_POINT_LIGHTS_PER_VERTEX 8
#define MAX_NUM_SPOT_LIGHTS_PER_VERTEX 4
#define MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX 4

#define POINT_LIGHT_CUTOFF_DISTANCE 300
#define SPOT_LIGHT_CUTOFF_DISTANCE 300
#define AMBIENT_LIGHT_CUTOFF_DISTANCE 1000

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
    float padding1;
    float padding2;
    float padding3;
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

out vertexShaderOutput
{
    vec3 position;
    mat3 TBN;
    vec2 uv;

    vec3 tangentFragmentPosition;
    vec3 tangentViewPosition;
    
    flat int numPointLights;
    flat int usedPointLightIndices[MAX_NUM_POINT_LIGHTS_PER_VERTEX];
    vec3 tangentSpacePointLights[MAX_NUM_POINT_LIGHTS_PER_VERTEX];

    flat int numSpotLights;
    flat int usedSpotLightIndices[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceSpotLights[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];

    flat int numAmbientLights;
    flat int usedAmbientLightIndices[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceAmbientLights[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
};

// calculate the final vertex transformation from affecting bones
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
mat3 CalculateInverseTBN(vec3 transNormal)
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 tangent = normalize(normalMatrix * vertexTangent);
    vec3 bitangent = normalize(normalMatrix * vertexBitangent);
    vec3 normal = normalize(normalMatrix * vertexNormal);
    tangent = normalize(tangent - dot(tangent, normal) * normal);

    return transpose(mat3(tangent, bitangent, normal));
}

// go through all point lights and check for ones whose light will have a significant contribution to the vertex, record their indices into the light stack
void ProcessPointLights(inout int numPointLights, inout int usedPointLightIndices[MAX_NUM_POINT_LIGHTS_PER_VERTEX], in vec3 animatedPos)
{
    numPointLights = 0;
    const int lightLength = pointLights.length();

    for (int i = 0; i < MAX_NUM_POINT_LIGHTS_PER_VERTEX; i++)
        usedPointLightIndices[i] = -1;

    for (int i = 0; i < lightLength; i++) {
        // calculate the distance
        float lightDistance = distance(pointLights[i].position.xyz, animatedPos);

        // check if the light actually contributes
        if (lightDistance <= POINT_LIGHT_CUTOFF_DISTANCE) {
            // if we haven't rached max, just add it
            if (numPointLights < MAX_NUM_POINT_LIGHTS_PER_VERTEX) {
                usedPointLightIndices[numPointLights] = i;
                numPointLights += 1;
            }
            // if we have reached max, then figure which light we need to replace, if any
            else {
                // index of the light to be replaced
                int index = -1;
                float maxDelta = 0.0f;

                // go through all lights we have so far, calculate and store the index of the one with the weakest contribution
                for (int j = 0; j < MAX_NUM_POINT_LIGHTS_PER_VERTEX; j++) {
                    float delta = lightDistance - distance(pointLights[usedPointLightIndices[j]].position.xyz, animatedPos);

                    if (delta > maxDelta) {
                        maxDelta = delta;
                        index = j;
                    }
                }
                // replace the light with weaker contribution than ours would be
                if (index >= 0)
                    usedPointLightIndices[index] = i;
            }
        }
    }
}

// go through all spot lights and check for ones whose light will have a significant contribution to the vertex, record its index into the light stack
void ProcessSpotLights(inout int numSpotLights, inout int usedSpotLightIndices[MAX_NUM_SPOT_LIGHTS_PER_VERTEX], in vec3 animatedPos)
{
    numSpotLights = 0;
    const int lightLength = spotLights.length();

    for (int i = 0; i < MAX_NUM_SPOT_LIGHTS_PER_VERTEX; i++)
        usedSpotLightIndices[i] = -1;

    for (int i = 0; i < lightLength; i++) {
        // calculate the distance
        float lightDistance = distance(spotLights[i].position.xyz, animatedPos);

        // check if the light actually contributes
        if (lightDistance <= SPOT_LIGHT_CUTOFF_DISTANCE) {
            // if we haven't rached max, just add it
            if (numSpotLights < MAX_NUM_SPOT_LIGHTS_PER_VERTEX) {
                usedSpotLightIndices[numSpotLights] = i;
                numSpotLights += 1;
            }
            // if we have reached max, then figure which light we need to replace, if any
            else {
                // index of the light to be replaced
                int index = -1;
                float maxDelta = 0.0f;

                // go through all lights we have so far, calculate and store the index of the one with the weakest contribution
                for (int j = 0; j < MAX_NUM_SPOT_LIGHTS_PER_VERTEX; j++) {
                    float delta = lightDistance - distance(spotLights[usedSpotLightIndices[j]].position.xyz, animatedPos);

                    if (delta > maxDelta) {
                        maxDelta = delta;
                        index = j;
                    }
                }
                // replace the light with weaker contribution than ours would be
                if (index >= 0)
                    usedSpotLightIndices[index] = i;
            }
        }
    }
}
// go through all ambient lights and check for ones whose light will have a significant contribution to the vertex, record its index into the light stack
void ProcessAmbientLights(inout int numAmbientLights, inout int usedAmbientLightIndices[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX], in vec3 animatedPos)
{
    numAmbientLights = 0;
    const int lightLength = ambientLights.length();

    for (int i = 0; i < MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX; i++)
        usedAmbientLightIndices[i] = -1;

    for (int i = 0; i < lightLength; i++) {
        // calculate the distance
        float lightDistance = distance(ambientLights[i].position.xyz, animatedPos);

        // check if the light actually contributes
        if (lightDistance <= AMBIENT_LIGHT_CUTOFF_DISTANCE) {
            // if we haven't rached max, just add it
            if (numAmbientLights < MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX) {
                usedAmbientLightIndices[numAmbientLights] = i;
                numAmbientLights += 1;
            }
            // if we have reached max, then figure which light we need to replace, if any
            else {
                // index of the light to be replaced
                int index = -1;
                float maxDelta = 0.0f;

                // go through all lights we have so far, calculate and store the index of the one with the weakest contribution
                for (int j = 0; j < MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX; j++) {
                    float delta = lightDistance - distance(ambientLights[usedAmbientLightIndices[j]].position.xyz, animatedPos);

                    if (delta > maxDelta) {
                        maxDelta = delta;
                        index = j;
                    }
                }
                // replace the light with weaker contribution than ours would be
                if (index >= 0)
                    usedAmbientLightIndices[index] = i;
            }
        }
    } 
}

void main()
{
    mat4 animatedTransform = BoneTransformation(boneIDs, boneWeights, boneTransformations);
    vec4 animatedPosition = animatedTransform * vec4(vertexPosition, 1.0f);
    vec3 animatedNormal = (animatedTransform * vec4(vertexNormal, 0.0f)).xyz;

    gl_Position =  projection * view * model * animatedPosition;

    // output
    position = animatedPosition.xyz;
    TBN = CalculateInverseTBN(animatedNormal);
    uv = vertexCoordinate;

    // light processing
    ProcessPointLights(numPointLights, usedPointLightIndices, animatedPosition.xyz);
    //ProcessSpotLights(numSpotLights, usedSpotLightIndices, animatedPosition.xyz);
    //ProcessAmbientLights(numAmbientLights, usedAmbientLightIndices, animatedPosition.xyz);
    
    // tangent space output
    tangentFragmentPosition = TBN * position;
    tangentViewPosition = TBN * vec3(view[0][3], view[1][3], view [2][3]);
    
    for (int i = 0; i < numPointLights; i++)
        tangentSpacePointLights[i] = TBN * pointLights[usedPointLightIndices[i]].position.xyz;
    
    for (int i = 0; i < numSpotLights; i++)
        tangentSpaceSpotLights[i] = TBN * spotLights[usedSpotLightIndices[i]].position.xyz;
    /*
    for (int i = 0; i < numAmbientLights; i++)
        tangentSpaceAmbientLights[i] = TBN * ambientLights[usedAmbientLightIndices[i]].position.xyz;*/
}
