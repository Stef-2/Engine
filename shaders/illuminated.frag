#version 460 core

in vertOutput
{
    vec3 position;
    vec3 normal;
    vec2 uv;
};

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

out vec4 fragColor;

uniform sampler2D texture;

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

void main()
{
    float roughness = 1f;
    vec4 color = texture2D(texture, uv);

    vec3 viewDirection = normalize(-vec3(view[0][2], view[1][2], view[2][2]));
    vec3 lightDirection = normalize(spotLights[0].position.xyz - position);
    vec3 reflectDirection = reflect(lightDirection, normal);
    float lightFacingRatio = max(dot(normal, lightDirection), 0.0f);

    vec3 flashLightLD = normalize(-vec3(view[3][0], view[3][1], view[3][2]) - position);
    float flashLightTheta = dot(flashLightLD, -viewDirection);
    float theta = dot(lightDirection, normalize(-spotLights[0].rotation.xyz));
    float cutOff = cos(radians(12.5f));
    float outerCutOff = cos(radians(15.0f));
    float epsilon = cutOff - outerCutOff;
    float smoothIntensity = clamp((flashLightTheta - outerCutOff) / epsilon, 0.0, 1.0);
    
    float intensity = spotLights[0].intensity / pow(distance(position, spotLights[0].position.xyz), 2);
    
    //vec3 diffuse = vec3(0.0f);
    //vec3 specular = vec3(0.0f);


    //vec3 diffuse = color.rgb * lightFacingRatio * spotLights[0].color.rgb * intensity;
    //vec3 specular = roughness * pow(max(dot(viewDirection, reflectDirection), 0.0), 32) * spotLights[0].color.rgb * intensity;
    //diffuse *= smoothIntensity;
    //specular *= smoothIntensity;

    vec3 diffuse = directionalLights[0].intensity * max(dot(normal, directionalLights[0].rotation.xyz), 0.0f) * directionalLights[0].color.rgb;
    diffuse += ambientLights[0].intensity * ambientLights[0].color.rgb / distance(position, ambientLights[0].position.xyz);
    

    fragColor = color * vec4(diffuse, 1.0f);

}
