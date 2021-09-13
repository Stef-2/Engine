#version 460 core

in vec3 normal;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D texture;

uniform mat4 view;

void main()
{
    vec4 color = texture2D(texture, uv);
    fragColor = color;
}
