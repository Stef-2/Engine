#version 460 core

layout (location = 11) in vec3 instancedPosition;

layout (binding = 0, std140) uniform mvpMatrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main()
{   
    gl_Position = projection * view * vec4(instancedPosition, 1.0f);
}
