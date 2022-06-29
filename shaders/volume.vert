#version 460 core

layout (location = 0) in vec3 vertexPosition;

layout (binding = 0, std140) uniform mvpMatrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vertexOutput
{
    vec4 position;
    flat vec3 viewDirection;
    vec4 rayDirection;

    vec4 WSpos;
    flat vec4 WSviewPos;
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
    vec4 viewPos = -view[3] * view;
    //viewPos /= viewPos.w;
    WSpos = model * vec4(vertexPosition, 1.0f);
    WSviewPos = viewPos;
    //viewPos = (vec4(-view[3].xyz, 1.0f) * view).xyz;
    //viewPos = -projView[3].xyz * mat3(projView);
    //vec3 viewPos = vec3(view[0][3], view[1][3], view[2][3]) * mat3(view);

    //viewDirection = vec3(view[0][2], view[1][2], view [2][2]);

    rayDirection = normalize(position - viewPos) * projection * view;
    viewDirection = normalize(view[2].xyz);

    //rayDirection = (projection * vec4(rayDirection, 1.0f)).xyz;
    //position = (projection * vec4(position, 1.0f)).xyz;
}
