#version 460 core

#define MAX_BONES_PER_VERTEX 4

#define MAX_NUM_POINT_LIGHTS_PER_VERTEX 4
#define MAX_NUM_SPOT_LIGHTS_PER_VERTEX 2
#define MAX_DIRECTIONAL_LIGHTS_PER_VERTEX 2
#define MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX 2

#define POINT_LIGHT_CUTOFF_DISTANCE 300
#define SPOT_LIGHT_CUTOFF_DISTANCE 300
#define AMBIENT_LIGHT_CUTOFF_DISTANCE 1000

const mat4 ones4x4 = mat4(vec4(1.0f), vec4(1.0f), vec4(1.0f), vec4(1.0f));
const mat3 ones3x3 = mat3(vec3(1.0f), vec3(1.0f), vec3(1.0f));

// input
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
    float padding[3];
};

struct SpotLight {
    vec4 position;
    vec4 orientation;
    vec4 color;
    float intensity;
    float angle;
    float sharpness;
    float padding[1];
};

struct DirectionalLight {
    vec4 position;
    vec4 orientation;
    vec4 color;
    float intensity;
    float padding[3];
};

struct AmbientLight {
    vec4 position;
    vec4 color;
    float intensity;
    float padding[3];
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

// shader flag reference
const unsigned int SHADER_STATIC      = 1;
const unsigned int SHADER_ANIMATED    = 2;
const unsigned int SHADER_ILLUMINATED = 3;
const unsigned int SHADER_SKYBOX      = 4;
const unsigned int SHADER_WIREFRAME   = 5;
const unsigned int SHADER_BASIC       = 6;

// uniforms
uniform unsigned int shaderFlags;
uniform mat4 boneTransformations[100];

// output
out vertexShaderBasicOutput
{
    vec3 position;
    vec2 uv;

    vec3 tangentFragmentPosition;
    vec3 tangentViewDirection;
};

out vertexShaderLightOutput
{
    flat int numPointLights;
    flat int usedPointLightIndices[MAX_NUM_POINT_LIGHTS_PER_VERTEX];
    vec3 tangentSpacePointLightPositions[MAX_NUM_POINT_LIGHTS_PER_VERTEX];

    flat int numSpotLights;
    flat int usedSpotLightIndices[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceSpotLightPositions[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceSpotLightOrientations[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];

    vec3 tangentSpaceDirectionalLightOrientations[MAX_DIRECTIONAL_LIGHTS_PER_VERTEX];

    flat int numAmbientLights;
    flat int usedAmbientLightIndices[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceAmbientLightPositions[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
};

// calculate the final vertex transformation from affecting bones
mat4 BoneTransformation(ivec4 boneIDs, vec4 boneWeights, mat4 boneTransformations[100])
{
    mat4 transform = mat4(0.0f);

    // run through all affecting bones and collect their transforms
    for(int i = 0 ; i < MAX_BONES_PER_VERTEX ; i++)
    {
        // we mark unused bone slots with -1, skip those
        if(boneIDs[i] == -1) 
            continue;

        transform += boneTransformations[boneIDs[i]] * boneWeights[i];
    }
    //return mat4(1.0f);
    return transform;
}

// inverse Tangent, Bitangent, Normal matrix for lighting calculations in fragment shader
mat3 CalculateTBN(in mat4 trans)
{
    mat4 transformedModelMatrix = model * trans;

    vec3 tangent = normalize(vec3(transformedModelMatrix * vec4(vertexTangent, 0.0f)));
    vec3 bitangent = normalize(vec3(transformedModelMatrix * vec4(vertexBitangent, 0.0f)));
    vec3 normal = normalize(vec3(transformedModelMatrix * vec4(vertexNormal, 0.0f)));
    
    // inverse aka transpose because we'll be using this to transform other vectors into tangent space
    mat3 TBN = transpose(mat3(tangent, -bitangent, normal));

    return TBN;
}

// go through all point lights and check for ones whose light will have a significant contribution to the vertex, record their indices into the light stack
void ProcessPointLights(in vec3 animatedPos)
{
    numPointLights = 0;
    const int lightLength = pointLights.length();

    // prefill the used lights stack with -1 in case we need to do sanity checks later on
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
            // if we have reached max, then figure out which light we need to replace, if any
            else {
                // index of the light to be replaced
                int index = -1;
                float maxDelta = 0.0f;

                // go through all lights we have so far, calculate and store the index of the one with the weakest contribution among them
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
void ProcessSpotLights(in vec3 animatedPos)
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
void ProcessAmbientLights(in vec3 animatedPos)
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

void ProcessLights(in vec3 position, in mat3 tbn)
{
    ProcessPointLights(position);
    ProcessSpotLights(position);
    ProcessAmbientLights(position);

    for (int i = 0; i < numPointLights; i++)
        tangentSpacePointLightPositions[i] = tbn * pointLights[usedPointLightIndices[i]].position.xyz;
    
    for (int i = 0; i < numSpotLights; i++) {
        tangentSpaceSpotLightPositions[i] = tbn * spotLights[usedSpotLightIndices[i]].position.xyz;
        tangentSpaceSpotLightOrientations[i] = tbn * spotLights[usedSpotLightIndices[i]].orientation.xyz;
    }
    
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS_PER_VERTEX; i++)
        tangentSpaceDirectionalLightOrientations[i] = tbn * directionalLights[i].orientation.xyz;

    for (int i = 0; i < numAmbientLights; i++)
        tangentSpaceAmbientLightPositions[i] = tbn * ambientLights[usedAmbientLightIndices[i]].position.xyz;
}

void main()
{
    mat3 TBN;

    if (shaderFlags == SHADER_ANIMATED)
    {
        mat4 animatedTransform = BoneTransformation(boneIDs, boneWeights, boneTransformations);
        vec4 animatedPosition = animatedTransform * vec4(vertexPosition, 1.0f);

        gl_Position =  projection * view * model * animatedPosition;

        position = animatedPosition.xyz;
        TBN = CalculateTBN(animatedTransform);
        ProcessLights(animatedPosition.xyz, TBN);
    }

    else if (shaderFlags == SHADER_STATIC)
    {
        gl_Position =  projection * view * model * vec4(vertexPosition, 1.0f);
        position = (model * vec4(vertexPosition, 1.0f)).xyz;
        TBN = CalculateTBN(mat4(1.0f));
        ProcessLights(position, TBN);
    }

    uv = vertexCoordinate;
    tangentFragmentPosition = TBN * position;
    tangentViewDirection = TBN * vec3(view[0][2], view[1][2], view [2][2]);
}
