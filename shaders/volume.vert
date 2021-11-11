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
    vec3 position;
    flat vec3 viewDirection;
     vec3 rayDirection;
};

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);

    //mat4 inverseView = inverse(view);

    position = (model * vec4(vertexPosition, 1.0f)).xyz;
    vec3 viewPos = -view[3].xyz * mat3(view);
    //vec3 viewPos = vec3(view[0][3], view[1][3], view[2][3]) * mat3(view);

    //viewDirection = vec3(view[0][2], view[1][2], view [2][2]);

    rayDirection = normalize(position - viewPos);

    //rayDirection = (projection * vec4(rayDirection, 1.0f)).xyz;
    //position = (projection * vec4(position, 1.0f)).xyz;
}
