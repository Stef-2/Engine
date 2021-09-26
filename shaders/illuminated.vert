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

out vertOutput
{
    vec3 position;
    mat3 TBN;
    vec2 uv;
    
    flat int numPointLights;
    flat int usedPointLightIndices[MAX_NUM_POINT_LIGHTS_PER_VERTEX];
    vec4 tangentSpacePointLights[MAX_NUM_POINT_LIGHTS_PER_VERTEX];

    int numSpotLights;
    int usedSpotLightIndices[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];
    vec4 tangentSpaceSpotLights[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];

    int numAmbientLights;
    int usedAmbientLightIndices[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
    vec4 tangentSpaceAmbientLights[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
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
    vec3 tangent = normalize(vec3(model * vec4(vertexTangent, 0.0)));
    vec3 bitangent = normalize(vec3(model * vec4(vertexBitangent, 0.0)));
    vec3 normal = normalize(vec3(model * vec4(transNormal, 0.0)));

    return transpose(mat3(tangent, bitangent, normal));
}
/*
// go through all point lights and check for ones whose light will have a significant contribution to the vertex, record their indices into the light stack
void ProcessPointLights(inout int numPointLights, inout int usedPointLightIndices[MAX_NUM_POINT_LIGHTS_PER_VERTEX], in vec3 animatedPos)
{
    numPointLights = 0;
    const int lightLength = pointLights.length() + 1;

    for (int i = 0; i < MAX_NUM_POINT_LIGHTS_PER_VERTEX; i++)
        usedPointLightIndices[i] = -1;

    for (int i = 0; i < lightLength; i++) {
        // calculate the distance
        float lightDistance = distance(animatedPos, pointLights[i].position.xyz);

        // check if the light actually contributes
        if (lightDistance <= 100.0f) {
            // if we haven't rached max, just add it
            if (numPointLights < MAX_NUM_POINT_LIGHTS_PER_VERTEX) {
                usedPointLightIndices[numPointLights] = i;
                numPointLights += 1;
            }
            // if we have reached max, then figure which light we need to replace, if any
            else {
                // index of the light to be replaced
                float maxDelta = 0.0f;
                int index = 0;
                // go through all lights we have so far, calculate and store the index of the one with the weakest contribution
                for (int j = 0; j < MAX_NUM_POINT_LIGHTS_PER_VERTEX; j++) {
                    float delta = lightDistance - distance(pointLights[usedPointLightIndices[j]].position.xyz, animatedPos);

                    if (delta > maxDelta) {
                        maxDelta = delta;
                        index = j;
                    }
                }

                usedPointLightIndices[index] = i;
            }
        }
    }
        
}
*/
// go through all spot lights and check for ones whose light will have a significant contribution to the vertex, record its index into the light stack
void ProcessSpotLights(inout int numSpotLights, inout int usedSpotLightIndices[MAX_NUM_SPOT_LIGHTS_PER_VERTEX], in vec3 animatedPos)
{
    numSpotLights = 0;
    const int lightLength = spotLights.length();

    float distances[lightLength];
    int indicies[lightLength];

    for (int i = 0; i < lightLength; i++) {
        // calculate and collect all intensities at this vertex
        distances[i] = distance(animatedPos, spotLights[i].position.xyz);
        indicies[i] = i;
        }
        
    // bitonic sort the stack of intensities so the most impactful ones are at the start
    for (int k = 2; k <= lightLength; k *= 2)
        for (int j = k/2; j > 0; j /= 2)
            for (int i = 0; i < lightLength; i++) {
            int l = i ^ j;
            if (l > i)
                if ((int(i ^ k) == 0) && (distances[i] > distances[l]) ||
                    (int(i ^ k) != 0) && (distances[i] < distances[l]))
                    {
                        int ihelp = indicies[i];
                        indicies[i] = indicies[l];
                        indicies[l] = ihelp;

                        float fhelp = distances[i];
                        distances[i] = distances[l];
                        distances[l] = fhelp;
                    }
                        
            }

    // pass the indices if they're above threshold, otherwise break since they're in descending order anyway so further evaluation makes no sense
    for (int i = 0; i < MAX_NUM_SPOT_LIGHTS_PER_VERTEX; i++)
        if (distances[i] <= 10.0f){
            usedSpotLightIndices[numPointLights] = indicies[i];
            numPointLights += 1;
            }
        else
            break;
}
// go through all ambient lights and check for ones whose light will have a significant contribution to the vertex, record its index into the light stack
void ProcessAmbientLights(inout int numAmbientLights, inout int usedAmbientLightIndices[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX], in vec3 animatedPos)
{
    numAmbientLights = 0;
    const int lightLength = ambientLights.length();

    float distances[lightLength];
    int indicies[lightLength];

    for (int i = 0; i < lightLength; i++) {
        // calculate and collect all intensities at this vertex
        distances[i] = distance(animatedPos, ambientLights[i].position.xyz);
        indicies[i] = i;
        }
        
    // bitonic sort the stack of intensities so the most impactful ones are at the start
    for (int k = 2; k <= lightLength; k *= 2)
        for (int j = k/2; j > 0; j /= 2)
            for (int i = 0; i < lightLength; i++) {
            int l = i ^ j;
            if (l > i)
                if ((int(i ^ k) == 0) && (distances[i] > distances[l]) ||
                    (int(i ^ k) != 0) && (distances[i] < distances[l]))
                    {
                        int ihelp = indicies[i];
                        indicies[i] = indicies[l];
                        indicies[l] = ihelp;

                        float fhelp = distances[i];
                        distances[i] = distances[l];
                        distances[l] = fhelp;
                    }
                        
            }

    // pass the indices if they're above threshold, otherwise break since they're in descending order anyway so further evaluation makes no sense
    for (int i = 0; i < MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX; i++)
        if (distances[i] <= ambientLights[i].intensity){
            usedAmbientLightIndices[numPointLights] = indicies[i];
            numPointLights += 1;
            }
        else
            break;
}

void main()
{
    mat4 animatedTransform = BoneTransformation(boneIDs, boneWeights, boneTransformations);
    vec4 animatedPosition = animatedTransform * vec4(vertexPosition, 1.0f);
    vec3 animatedNormal = (animatedTransform * vec4(vertexNormal, 0.0f)).xyz;

    gl_Position =  projection * view * model * animatedPosition;

    position = animatedPosition.xyz;
    TBN = CalculateInverseTBN(animatedNormal);
    uv = vertexCoordinate;

    //ProcessPointLights(numPointLights, usedPointLightIndices, animatedPosition.xyz);
    //ProcessSpotLights(numSpotLights, usedSpotLightIndices, animatedPosition.xyz);
    //ProcessAmbientLights(numAmbientLights, usedAmbientLightIndices, animatedPosition.xyz);
}
