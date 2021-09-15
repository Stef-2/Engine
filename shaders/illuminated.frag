#version 460 core

in vertOutput
{
    vec3 normal;
    vec2 uv;
};

out vec4 fragColor;

uniform sampler2D texture;

uniform mat4 view;

void main()
{
    vec4 color = texture2D(texture, uv);
    fragColor = color * dot(normal, vec3(view[0][2], view[1][2], view[2][2]));
}
