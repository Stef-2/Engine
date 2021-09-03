#version 460 core

layout (location = 0) in vec3 vertexPosition;

out vec3 uv;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    uv = vertexPosition;
    vec4 position = projection * view * vec4(vertexPosition, 1.0f);
    gl_Position = position.xyww;
}
