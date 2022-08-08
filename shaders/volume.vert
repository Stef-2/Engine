#version 460 core

layout (location = 0) in vec3 vertexPosition;

layout (binding = 0, std140) uniform mvpMatrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

struct OutputData
{
    mat4 mats[8];
    vec4 vecs[8];
    float floats[8];
};

layout (binding = 69, std430) buffer Debug {
mat4 mats[8];
vec4 vecs[8];
float floats[8];
};

out vertexOutput
{
    vec4 position;
    flat vec3 viewDirection;
    vec4 rayDirection;

    vec4 WSpos;
    vec4 WSviewPos;
    flat mat4 pw;
};

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);

    //mat4 inverseView = inverse(view);
    mat4 projView = projection * view;
    pw = projView;
    position = model * vec4(vertexPosition, 1.0f);
    //position = gl_Position.xyz;
    vec4 viewPos = (-view[3] * view);
    //viewPos[3] = 1;
    //viewPos /= viewPos.w;
    WSpos = model * vec4(vertexPosition, 0);
    //WSpos.w = viewPos.w;
    //WSpos[3] = viewPos[3];
    WSviewPos = viewPos;
    //viewPos = (vec4(-view[3].xyz, 1.0f) * view).xyz;
    //viewPos = -projView[3].xyz * mat3(projView);
    //vec3 viewPos = vec3(view[0][3], view[1][3], view[2][3]) * mat3(view);

    //viewDirection = vec3(view[0][2], view[1][2], view [2][2]);
    WSviewPos.w = 1;
    rayDirection = normalize((WSpos) - (WSviewPos));
    viewDirection = normalize(view[2].xyz);
    vecs[0] = rayDirection;
    //rayDirection = (projection * vec4(rayDirection, 1.0f)).xyz;
    //position = (projection * vec4(position, 1.0f)).xyz;
}
