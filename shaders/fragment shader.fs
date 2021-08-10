#version 120

varying vec3 normal;
varying vec2 texCoord;

uniform sampler2D tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 color = texture2D(tex, texCoord);
    gl_FragColor = color * pow(dot(normal, vec3(view[0][2], view[1][2], view[2][2])), 2);
}
