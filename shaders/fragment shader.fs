#version 120

varying vec2 texCoord;

uniform sampler2D tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 color = texture2D(tex,texCoord);
    gl_FragColor = color;
}
