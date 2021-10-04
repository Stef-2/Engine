#version 460 core

#define SPECULAR_EXPONENT 32

#define MAX_NUM_POINT_LIGHTS_PER_VERTEX 8
#define MAX_NUM_SPOT_LIGHTS_PER_VERTEX 4
#define MAX_DIRECTIONAL_LIGHTS_PER_VERTEX 2
#define MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX 4

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

// input
in vertexShaderBasicOutput
{
    vec3 position;
    vec2 uv;

    vec3 tangentFragmentPosition;
    vec3 tangentViewDirection;
};

in vertexShaderLightOutput
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

out vec4 fragColor;

// shader flag reference
const unsigned int SHADER_STATIC      = 1;
const unsigned int SHADER_ANIMATED    = 2;
const unsigned int SHADER_ILLUMINATED = 3;
const unsigned int SHADER_SKYBOX      = 4;
const unsigned int SHADER_WIREFRAME   = 5;
const unsigned int SHADER_BASIC       = 6;

// uniforms
uniform unsigned int shaderFlags;
uniform samplerCube cubeMap;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

vec4 diffuseReflection = vec4(0.0f);
vec4 specularReflection = vec4(0.0f);

mat2x4 ProcessPointLights(in vec3 normal)
{
    mat2x4 result = {vec4(0.0f), vec4(0.0f)};
    float roughness = 1.0f;

    for (unsigned int i = 0; i < 2; i++)
    {
        vec3 lightDirection = normalize(tangentSpacePointLightPositions[i] - tangentFragmentPosition);
        float lightFacingRatio = max(dot(normal, lightDirection), 0.0f);
        float intensity = pointLights[usedPointLightIndices[i]].intensity / pow(distance(tangentFragmentPosition, tangentSpacePointLightPositions[i]), 2);
        // blinn-phong specular model
        vec3 blinnDirection = normalize(lightDirection + tangentViewDirection);

        result[0] += lightFacingRatio * pointLights[usedPointLightIndices[i]].color * intensity;
        result[1] += pow(max(dot(normal, blinnDirection), 0.0), SPECULAR_EXPONENT) * pointLights[usedPointLightIndices[i]].color * intensity;
    }
    
    return result;
}

mat2x4 ProcessSpotLights(in vec3 normal)
{
    mat2x4 result = {vec4(0.0f), vec4(0.0f)};
    const float spreadAngle = 5.0f;
    float roughness = 1.0f;

    for (unsigned int i = 0; i < numSpotLights; i++)
    {
        vec3 lightDirection = normalize(tangentSpaceSpotLightPositions[i] - tangentFragmentPosition);
        float lightFacingRatio = max(dot(normal, lightDirection), 0.0f);
        float intensity = pointLights[usedPointLightIndices[i]].intensity / pow(distance(tangentFragmentPosition, tangentSpaceSpotLightPositions[i]), 2);
        // blinn-phong specular model
        vec3 blinnDirection = normalize(lightDirection + tangentViewDirection);

        float theta = dot(lightDirection, normalize(-spotLights[usedSpotLightIndices[i]].orientation.xyz));
        float innerCutoff = cos(radians(spotLights[usedSpotLightIndices[i]].angle) - (spreadAngle * spotLights[usedSpotLightIndices[i]].sharpness));
        float outterCutoff = cos(radians(spotLights[usedSpotLightIndices[i]].angle) + (spreadAngle * spotLights[usedSpotLightIndices[i]].sharpness));
        float epsilon   = innerCutoff - outterCutoff;
        intensity *= clamp((theta - outterCutoff) / epsilon, 0.0, 1.0); 

        result[0] += lightFacingRatio * spotLights[usedSpotLightIndices[i]].color * intensity;
        result[1] += pow(max(dot(normal, blinnDirection), 0.0), SPECULAR_EXPONENT) * spotLights[usedSpotLightIndices[i]].color * intensity;
    }
    
    return result;
}

mat2x4 ProcessDirectionalLights(in vec3 normal)
{
    mat2x4 result = {vec4(0.0f), vec4(0.0f)};

    for (unsigned int i = 0; i < directionalLights.length(); i++)
    {
        vec3 lightDirection = normalize(tangentSpaceDirectionalLightOrientations[i]);
        float lightFacingRatio = max(dot(normal, lightDirection), 0.0f);
        // blinn-phong specular model
        vec3 blinnDirection = normalize(lightDirection + tangentViewDirection);

        result[0] += lightFacingRatio * directionalLights[i].color * directionalLights[i].intensity;
        result[1] += pow(max(dot(normal, blinnDirection), 0.0), SPECULAR_EXPONENT) * directionalLights[i].color * directionalLights[i].intensity;
    }

    return result;
}

vec4 ProcessAmbientLights(in vec3 normal)
{
    vec4 result = vec4(0.0f);

     for (unsigned int i = 0; i < numAmbientLights; i++)
     {
        float intensity = ambientLights[usedAmbientLightIndices[i]].intensity / distance(position, ambientLights[usedAmbientLightIndices[i]].position.xyz);

        result += ambientLights[usedAmbientLightIndices[i]].color * intensity;
     }

     return result;
}

void main()
{
    vec4 diffuseSample = texture2D(diffuseMap, uv);
    vec3 normalSample = (texture(normalMap, uv).rgb * 2.0f - 1.0f);

    mat2x4 pointLightContributions = ProcessPointLights(normalSample);
    mat2x4 directionalLightsContributions = ProcessDirectionalLights(normalSample);

    diffuseReflection += pointLightContributions[0];// + directionalLightsContributions[0];
    specularReflection += pointLightContributions[1];// + directionalLightsContributions[1];

    fragColor =  diffuseSample * (diffuseReflection + specularReflection);
}
