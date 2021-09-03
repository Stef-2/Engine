#version 460 core

varying vec3 normal;
varying vec2 uv;

uniform sampler2D texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 color = texture2D(texture, uv);
    gl_FragColor = color * dot(normal, vec3(view[0][2], view[1][2], view[2][2]));
}
