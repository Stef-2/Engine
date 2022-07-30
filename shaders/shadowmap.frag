#version 460 core

layout (binding = 0, std140) uniform mvpMatrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main()
{             
    // dont need anything here
} 