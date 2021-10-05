#version 460 core

#define MAX_NUM_POINT_LIGHTS_PER_VERTEX 4
#define MAX_NUM_SPOT_LIGHTS_PER_VERTEX 2
#define MAX_DIRECTIONAL_LIGHTS_PER_VERTEX 2
#define MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX 2

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// input
in vertexShaderBasicOutput
{
    vec3 position;
    vec2 uv;

    vec3 tangentFragmentPosition;
    vec3 tangentViewDirection;
} basicInput[];

in vertexShaderLightOutput
{
    flat int numPointLights;
    flat int usedPointLightIndices[MAX_NUM_POINT_LIGHTS_PER_VERTEX];
    vec3 tangentSpacePointLightPositions[MAX_NUM_POINT_LIGHTS_PER_VERTEX];

    flat int numSpotLights;
    flat int usedSpotLightIndices[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceSpotLightPositions[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceSpotLightOrientations[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];

    vec3 tangentSpaceDirectionalLightOrientations[MAX_DIRECTIONAL_LIGHTS_PER_VERTEX];

    flat int numAmbientLights;
    flat int usedAmbientLightIndices[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceAmbientLightPositions[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
} lightInput[];

//output
out vertexShaderBasicOutput
{
    vec3 position;
    vec2 uv;

    vec3 tangentFragmentPosition;
    vec3 tangentViewDirection;
};

out vertexShaderLightOutput
{
    flat int numPointLights;
    flat int usedPointLightIndices[MAX_NUM_POINT_LIGHTS_PER_VERTEX];
    vec3 tangentSpacePointLightPositions[MAX_NUM_POINT_LIGHTS_PER_VERTEX];

    flat int numSpotLights;
    flat int usedSpotLightIndices[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceSpotLightPositions[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceSpotLightOrientations[MAX_NUM_SPOT_LIGHTS_PER_VERTEX];

    vec3 tangentSpaceDirectionalLightOrientations[MAX_DIRECTIONAL_LIGHTS_PER_VERTEX];

    flat int numAmbientLights;
    flat int usedAmbientLightIndices[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
    vec3 tangentSpaceAmbientLightPositions[MAX_NUM_AMBIENT_LIGHTS_PER_VERTEX];
};

void main() 
{   
    // assign the max of the 3 vertex numXlights to the whole triangle
    // apparently, this isn't even needed, the presence of geometry shader fixes it on its own
    /*
    int maxNumPointLights = lightInput[0].numPointLights;
    int maxNumSpotLights = lightInput[0].numSpotLights;
    int maxNumAmbientLights = lightInput[0].numAmbientLights;

    for (int i = 1; i < gl_in.length(); i++)
    {
        maxNumPointLights = max(maxNumPointLights, lightInput[i].numPointLights);
        maxNumSpotLights = max(maxNumSpotLights, lightInput[i].numSpotLights);
        maxNumAmbientLights = max(maxNumAmbientLights, lightInput[i].numAmbientLights);
    }
    */
    for (int i = 0; i < gl_in.length(); i++)
    {
        position = basicInput[i].position;
        uv = basicInput[i].uv;
        tangentFragmentPosition = basicInput[i].tangentFragmentPosition;
        tangentViewDirection = basicInput[i].tangentViewDirection;
        
        numPointLights = lightInput[i].numPointLights;
        numSpotLights = lightInput[i].numSpotLights;
        numAmbientLights = lightInput[i].numAmbientLights;
        
        /* apparently unneeded maxXlights fix
        numPointLights = maxNumPointLights;
        numSpotLights = maxNumSpotLights;
        numAmbientLights = maxNumAmbientLights;
        */
        usedPointLightIndices = lightInput[i].usedPointLightIndices;
        usedSpotLightIndices = lightInput[i].usedSpotLightIndices;
        usedAmbientLightIndices = lightInput[i].usedAmbientLightIndices;

        tangentSpacePointLightPositions = lightInput[i].tangentSpacePointLightPositions;
        tangentSpaceSpotLightPositions = lightInput[i].tangentSpaceSpotLightPositions;
        tangentSpaceSpotLightOrientations = lightInput[i].tangentSpaceSpotLightOrientations;
        tangentSpaceDirectionalLightOrientations = lightInput[i].tangentSpaceDirectionalLightOrientations;
        tangentSpaceAmbientLightPositions = lightInput[i].tangentSpaceAmbientLightPositions;

        gl_Position = gl_in[i].gl_Position;

        EmitVertex();
    }
    EndPrimitive();
}
