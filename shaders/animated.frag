#version 460 core

in vec3 position;
in vec3 normal;
in vec2 uv;
in flat ivec4 boneids;
in vec4 boneweights;

out vec4 fragColor;

uniform sampler2D texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 boneTransformations[100];
void main()
{
    mat4 totalTransform = mat4(1.0f);
    vec4 totalPosition = vec4(0.0f);

    for(int i = 0 ; i < 4 ; i++)
    {
        if(boneids[i] == -1) 
            continue;
        if(boneids[i] >= 100) 
        {
            totalPosition = vec4(position,1.0f);
            break;
        }
        totalTransform += boneTransformations[boneids[i]] * boneweights[i];
        //totalPosition += localPosition * boneWeights[i];
    }
    vec4 pos = totalTransform * vec4(position, 1.0f);
    vec4 color = texture2D(texture, uv);
    fragColor = color * dot(normal, vec3(view[0][2], view[1][2], view[2][2]));
    //fragColor = color * pos;
}
