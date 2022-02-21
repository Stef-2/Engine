#ifndef SHADER_H
#define SHADER_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "glfw3.h"
#include "glm/glm.hpp"

#include "fstream"
#include "sstream"
#include "string"
#include "iostream"
#include "memory"

namespace Engine
{
    // OpenGL shader program wrapper class
    // encapsulates vertex, geometry and fragment shaders as well as the shader program that binds them
    // performs compiling and linking of shader programs
    // holds the internal OpenGL locations of shader attribute and buffer locations so we don't have to query the GPU for them
    // provides easy retrieval of the currently active shader program
    class Shader
    {
        public:

            // enumerator for the different shader attribute we may need to retrive
            enum class ShaderAttribute
            {
                VERTEX_POSITION_LOCATION,
                VERTEX_NORMAL_LOCATION,
                VERTEX_UV_LOCATION,
                VERTEX_BONE_ID_LOCATION,
                VERTEX_BONE_WEIGHTS_LOCATION,
                MODEL_LOCATION,
                VIEW_LOCATION,
                PROJECTION_LOCATION,
                BONE_TRANSFORMATIONS,
                SHADER_PARAMETERS,
                DIFFUSE_MAP,
                ROUGHNESS_MAP,
                METALLIC_MAP,
                SPECULAR_MAP,
                NORMAL_MAP,
                ALPHA_MAP,
                CUBE_MAP,
                SHADOW_MAPS
            };

            // enumerator for different uniform or shader storage buffer objects
            enum class UniformBuffer
            {
                MVP_MATRICES,
                POINT_LIGHTS,
                SPOT_LIGHTS,
                DIRECTIONAL_LIGHTS,
                AMBIENT_LIGHTS,
                MATERIAL_PARAMETERS
            };

            // enumerator for different kinds of shader flags
            typedef enum class ShaderFlag : unsigned int
            {
                STATIC      = 0b0000'0001,
                ANIMATED    = 0b0000'0010,
                ILLUMINATED = 0b0000'0011,
                SKYBOX      = 0b0000'0100,
                WIREFRAME   = 0b0000'0101,
                BASIC       = 0b0000'0110
            };

            Shader();
            Shader(std::string vertexShader, std::string fragmentShader);
            Shader(std::string vertexShader, std::string geometryShader, std::string fragmentShader);

            // Shader(const Shader& other);
            // Shader& operator=(const Shader& other);
            // ~Shader();

            // glGetCurrentProgram() is very slow, just like all functions that ask the graphics processor for data
            // so we're storing it ourselves in case we need it
            static Shader& GetCurrentShader();

            unsigned int GetVertexShader();
            unsigned int GetGeometryShader();
            unsigned int GetFragmentShader();
            unsigned int GetProgramID();

            unsigned int GetAttributeLocation(ShaderAttribute attribute);
            static unsigned int GetUniformBuffer(UniformBuffer buffer);

            // handle shader flags for uber shader
            ShaderFlag& GetShaderFlags();
            bool GetShaderFlag(ShaderFlag flag);
            void SetShaderFlag(ShaderFlag flag);

            std::string GetLogData();

            // load vertex and fragment shaders from files
            int SetVertexShader(std::string filePath);
            int SetGeometryShader(std::string filePath);
            int SetFragmentShader(std::string filePath);

            // compiles vertex and fragment shaders into a program and binds it
            // needs to be run after every shader change
            int CompileProgram();

            // activates this shader program
            // needs to be run before every draw call that will make use of this shader
            void Activate();

        private:
            // will be set to 1 if shader compilation succeeds, 0 otherwise
            int compileSuccess;
            // vertex shader compile log
            char vsLog[512];
            // geometry shader compile log
            char gsLog[512];
            // fragment shader compile log
            char fsLog[512];
            // shader program compile log
            char spLog[512];

            // handles for the shaders and program themselves
            unsigned int vertexShader;
            unsigned int geometryShader;
            unsigned int fragmentShader;
            unsigned int programID;

            // shader attribute locations
            unsigned int vertexPositionLocation;
            unsigned int vertexNormalLocation;
            unsigned int vertexUvLocation;
            unsigned int vertexBoneIdLocation;
            unsigned int vertexBoneWeights;

            // shader uniform locations
            unsigned int modelTransformLocation;
            unsigned int viewTransformLocation;
            unsigned int projectionTransformLocation;
            unsigned int BoneTransformsLocation;
            unsigned int shaderFlagsLocation;

            // texture locations
            unsigned int diffuseMapLocation;
            unsigned int roughnessMapLocation;
            unsigned int metallicMapLocation;
            unsigned int specularMapLocation;
            unsigned int normalMapLocation;
            unsigned int alphaMapLocation;
            unsigned int cubeMapLocation;
            unsigned int shadowMapsLocation;

            // shader block locations
            static unsigned int mvpBlock;
            static unsigned int materialParametersBlock;
            static unsigned int pointLightsBLock;
            static unsigned int spotLightsBLock;
            static unsigned int directionalLightsBLock;
            static unsigned int ambientLightsBLock;

            ShaderFlag shaderFlags;
            static Shader* currentShader;
    };

}

#endif //  SHADER_H
