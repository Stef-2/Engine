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
  vec3 position;
  vec3 rotation;
  vec3 color;
  float intensity;
  float angle;
};

struct DirectionalLight {
    vec3 rotation;
    vec3 color;
    float intensity;
};

struct AmbientLight {
  vec3 position;
  vec3 color;
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
  SpotLight potLights[];
};

layout (binding = 3, std430) buffer DirectionalLights {
  DirectionalLight directionalLights[];
};

layout (binding = 4, std430) buffer AmbientLights {
  AmbientLight ambientLights[];
};

void main()
{
    PointLight plight;

    //for (int i = 0; i < pointLights.length(); i++)
        plight = pointLights[0];

    float roughness = 1f;
    vec4 color = texture2D(texture, uv);

    vec3 viewDirection = normalize(-vec3(view[0][2], view[1][2], view[2][2]));
    vec3 lightDirection = normalize(pointLights[0].position.xyz - position);
    vec3 reflectDirection = reflect(lightDirection, normal);
    float lightFacingRatio = max(dot(normal, lightDirection), 0.0f);

    float intensity = pointLights[0].intensity / pow(distance(position, pointLights[0].position.xyz), 2);
    
    vec3 diffuse = color.rgb * lightFacingRatio * pointLights[0].color.rgb * intensity;
    vec3 specular = roughness * pow(max(dot(viewDirection, reflectDirection), 0.0), 32) * pointLights[0].color.rgb * intensity;

    fragColor = color * vec4(diffuse + specular, 1.0f);

}
