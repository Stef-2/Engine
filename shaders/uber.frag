#version 460 core

#define SPECULAR_EXPONENT 32

#define MAX_NUM_POINT_LIGHTS_PER_VERTEX 4
#define MAX_NUM_SPOT_LIGHTS_PER_VERTEX 2
#define MAX_DIRECTIONAL_LIGHTS_PER_VERTEX 2
#define MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX 2

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

// basic input
in vertexShaderBasicOutput
{
    vec3 position;
    vec2 uv;

    vec3 tangentFragmentPosition;
    vec3 tangentViewDirection;
};

// light info input
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

// material texture samplers
uniform sampler2D diffuseMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;

uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D alphaMap;

uniform samplerCube cubeMap;

layout (binding = 5, std140) uniform materialParameters
{
    vec3 diffuseMultiplier;
	float roughnessMultiplier;
	float metallicMultiplier;

    float specularMultiplier;
    vec3 normalMultiplier;
	float alphaMultiplier;
};

// final diffuse and specular output
vec4 diffuseReflection = vec4(0.0f);
vec4 specularReflection = vec4(0.0f);

vec3 lightOutput = vec3(0.0f);

const float pi = 3.14159265359;
const float gamma = 2.2f;
const vec3 F0 = vec3(0.4f);

// ================================== PBR related functions =======================================

// GGX distribution model for rough surfaces
float GGXDistribution(vec3 normal, vec3 blinnDirection, float roughness)
{
    // apparently manual multiplication for small exponents (<9) beats pow()
    float facetRoughness = roughness * roughness * roughness * roughness;
    float ligtFacingRatio = max(dot(normal, blinnDirection), 0.0f);
    ligtFacingRatio = ligtFacingRatio * ligtFacingRatio;

    float denominator = (ligtFacingRatio * (facetRoughness - 1.0f) + 1.0f);
    denominator = pi * denominator * denominator;

    return facetRoughness / denominator;
}

// Schlick-Beckmann geometry function for approximating GGX distribution
float SchlickBeckmannGGX(float phongDirection, float roughness)
{
    float facetRoughness = ((roughness + 1.0f) * (roughness + 1.0f)) / 8.0f;

    float denominator = phongDirection * (1.0f - facetRoughness) + facetRoughness;

    return phongDirection / denominator;
}

// Smith geometry function for approximating GGX distribution
float SmithGeometryGGXApproximation(vec3 normal, vec3 viewDirection, vec3 lightDirection, float roughness)
{
    float phongDirection = max(dot(normal, viewDirection), 0.0f);
    float reflectDirection = max(dot(normal, lightDirection), 0.0f);
    float ggxDistribution1 = SchlickBeckmannGGX(phongDirection, roughness);
    float ggxDistribution2 = SchlickBeckmannGGX(reflectDirection, roughness);

    return ggxDistribution1 * ggxDistribution2;
}

// Shlick-Beckmann approximation of the Fresnel function
vec3 ShlickBeckmannFresnelFunction(float theta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - theta, 0.0f, 1.0f), 5.0f);
}

// ================================================================================================
/*
mat2x4 ProcessPointLights(in vec3 normal)
{
    mat2x4 result = {vec4(0.0f), vec4(0.0f)};
    float roughness = 1.0f;

    for (unsigned int i = 0; i < numPointLights; i++)
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
*/
void ProcessPointLights(in vec3 diffuse, in float roughness, in vec3 metallic, in vec3 normal, in vec3 skybox)
{
    vec3 fresnelZero = mix(F0, diffuse, metallic);
    
    for (unsigned int i = 0; i < numPointLights; i++)
    {
        vec3 lightDirection = normalize(tangentSpacePointLightPositions[i] - tangentFragmentPosition);
        // ratio that describes how much this fragment is normal to the light
        float lightFacingRatio = max(dot(normal, lightDirection), 0.0f);
        // light intensity at this fragment's position, following the inverse square intensity decay law
        float distance = distance(tangentFragmentPosition, tangentSpacePointLightPositions[i]);
        float intensity = pointLights[usedPointLightIndices[i]].intensity / (distance * distance);
        // blinn-phong specular model
        vec3 blinnDirection = normalize(lightDirection + tangentViewDirection);

        // PBR nonsense, god knows how any of this works
        float normalDistributionFunction = GGXDistribution(normal, blinnDirection, roughness);   
        float geometryFunction = SmithGeometryGGXApproximation(normal, tangentViewDirection, lightDirection, roughness);      
        vec3 fresnelFunction = ShlickBeckmannFresnelFunction(clamp(dot(blinnDirection, tangentViewDirection), 0.0f, 1.0f), fresnelZero);

        vec3 numerator = normalDistributionFunction * geometryFunction * fresnelFunction; 
        float denominator = 4.0f * max(dot(normal, tangentViewDirection), 0.0) * max(dot(normal, lightDirection), 0.0f) + 0.0001f;
        vec3 specular = numerator / denominator;

        vec3 specularContribution = fresnelFunction;
        // to ensure energy conservation, the total reflected values of diffuse + specular must not exceed 1.0f
        vec3 diffuseContribution = vec3(1.0f) - specularContribution;
        diffuseContribution = diffuseContribution * (1.0f - metallic);

        lightOutput += (diffuseContribution * diffuse / pi + specular) * (intensity * pointLights[usedPointLightIndices[i]].color.rgb) * lightFacingRatio;
    }
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

void ProcessDirectionalLights(in vec3 diffuse, in float roughness, in float metallic, in vec3 normal)
{   
    vec3 fresnelZero = mix(F0, diffuse, metallic);
    
    for (unsigned int i = 0; i < directionalLights.length(); i++)
    {
        vec3 lightDirection = normalize(tangentSpaceDirectionalLightOrientations[i]);
        // ratio that describes how much this fragment is normal to the light
        float lightFacingRatio = max(dot(normal, lightDirection), 0.0f);
        // blinn-phong specular model
        vec3 blinnDirection = normalize(lightDirection + tangentViewDirection);

        // PBR nonsense, god knows how any of this works
        float normalDistributionFunction = GGXDistribution(normal, blinnDirection, roughness);
        float geometryFunction = SmithGeometryGGXApproximation(normal, tangentViewDirection, lightDirection, roughness);      
        vec3 fresnelFunction = ShlickBeckmannFresnelFunction(clamp(dot(blinnDirection, tangentViewDirection), 0.0f, 1.0f), fresnelZero);

        vec3 numerator = normalDistributionFunction * geometryFunction * fresnelFunction; 
        float denominator = 4 * max(dot(normal, tangentViewDirection), 0.0) * max(dot(normal, lightDirection), 0.0f) + 0.0001f;
        vec3 specular = numerator / denominator;

        vec3 specularContribution = fresnelFunction;
        // to ensure energy conservation, the total reflected values of diffuse + specular must not exceed 1.0f
        vec3 diffuseContribution = vec3(1.0f) - specularContribution;
        diffuseContribution = diffuseContribution * (1.0f - metallic);

        lightOutput += (diffuseContribution * diffuse / pi + specular) * (directionalLights[i].intensity * pointLights[usedPointLightIndices[i]].color.rgb) * lightFacingRatio;
    }
}

void ProcessAmbientLights(in vec3 normal)
{
     for (unsigned int i = 0; i < numAmbientLights; i++)
     {
        float intensity = ambientLights[usedAmbientLightIndices[i]].intensity / distance(position, ambientLights[usedAmbientLightIndices[i]].position.xyz);

        lightOutput += ambientLights[usedAmbientLightIndices[i]].color * intensity;
     }
}

// extract the near and far clipping distances from the projection matrix
// there are bound to be some rounding errors creeping in here, especially for very large farclip values
// using double type for calculation should help to mitigate some of it
dvec2 NearAndFarClipValues()
{
    // element[2][2] is (far + near) / (far - near)
    double element22 = -projection[2][2];

    // element[3][2] is -(2 * near * far) / (far - near)
    double element32 = -projection[3][2];

    double far = ((2.0f * element32) / (2.0f * element22 - 2.0f));
    double near = ((element22 - 1.0f) * far) / (element22 + 1.0);

    return dvec2(near, far);
}

float LinearizeDepth() 
{
    dvec2 nearFar = NearAndFarClipValues();
    float z = gl_FragCoord.z * 2.0 - 1.0;

    float near = float(nearFar[0]);
    float far = float(nearFar[1]);

    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    vec4 diffuseSample = texture2D(diffuseMap, uv);
    vec4 roughnessSample = texture2D(roughnessMap, uv);
    vec4 metallicSample = texture2D(metallicMap, uv);
    vec4 normalSample = texture(normalMap, uv) * 2.0f - 1.0f;
    vec4 skyboxSample = texture(cubeMap, reflect(-tangentViewDirection, normalSample.rgb));

    vec3 diffuse = pow(diffuseSample.rgb, vec3(gamma));
    float roughness = (roughnessSample.r + roughnessSample.g + roughnessSample.b) / 3.0f;

    ProcessPointLights(diffuseSample.rgb , roughnessSample.r, vec3(metallicSample.r), normalSample.rgb, skyboxSample.rgb);
    //ProcessDirectionalLights(diffuseSample.rgb, 0.5f, 0.0f, normalSample.rgb);

    lightOutput += skyboxSample.rgb * (1 - roughnessSample.r);
    vec3 color = lightOutput;

    // HDR tonemapping
    //color = color / (color + vec3(1.0));
    // gamma correct
    //color = pow(color, vec3(1.0/gamma));

    //fragColor = skyboxSample;
    fragColor = vec4(color, 1.0f);
}
