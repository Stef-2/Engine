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
#include <vector>
#include <utility>

namespace Engine
{
    // OpenGL vertex shader
    class VertexShader
    {
    public:
        VertexShader();
        VertexShader(std::string filePath);

        void SetVertexShader(std::string filePath);
        int Compile();

        char* GetCompileLog();
        unsigned int GetVertexShader();

    private:
        char compilelog[512];
        unsigned int vertexShader;
    };

    // OpenGL geometry shader
    class GeometryShader
    {
	public:
        GeometryShader();
        GeometryShader(std::string filePath);

		void SetGeometryShader(std::string filePath);
		int Compile();

		char* GetCompileLog();
		unsigned int GetGeometryShader();

	private:
		char compilelog[512];
		unsigned int geometryShader;
    };

    // OpenGL fragment shader
    class FragmentShader
    {
	public:
        FragmentShader();
        FragmentShader(std::string filePath);

		void SetFragmentShader(std::string filePath);
		int Compile();

		char* GetCompileLog();
		unsigned int GetFragmentShader();

	private:
		char compilelog[512];
		unsigned int fragmentShader;
    };

    // OpenGL compute shader
    class ComputeShader
    {
	public:
        ComputeShader();
        ComputeShader(std::string filePath);

		void SetComputeShader(std::string filePath);
		int Compile();

		char* GetCompileLog();
		unsigned int GetComputeShader();

	private:
		char compilelog[512];
		unsigned int computeShader;
    };


    // OpenGL shader program wrapper class
    // encapsulates vertex, geometry and fragment shaders as well as the shader program that binds them
    // performs compiling and linking of shader programs
    // holds the internal OpenGL locations of shader attribute and buffer locations so we don't have to query the GPU for them
    // provides easy retrieval of the currently active shader program
    class Shader
    {
        public:
			// enumerator for the different shader attribute we may need to retrieve
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
				VOLUME_MAP,
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
				STATIC = 0b0000'0001,
				ANIMATED = 0b0000'0010,
				ILLUMINATED = 0b0000'0011,
				SKYBOX = 0b0000'0100,
				WIREFRAME = 0b0000'0101,
				BASIC = 0b0000'0110
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

            unsigned int GetAttributeLocation(Engine::Shader::ShaderAttribute attribute);
            static unsigned int GetUniformBuffer(Engine::Shader::UniformBuffer buffer);

            // handle shader flags for uber shader
            Engine::Shader::ShaderFlag& GetShaderFlags();
            bool GetShaderFlag(Engine::Shader::ShaderFlag flag);
            void SetShaderFlag(Engine::Shader::ShaderFlag flag);

            std::string GetLogData();

            // load vertex, geometry and fragment shaders from files
            int SetVertexShader(std::string filePath);
            int SetGeometryShader(std::string filePath);
            int SetFragmentShader(std::string filePath);

            // load vertex, geometry and fragment shaders from objects
			int SetVertexShader(Engine::VertexShader& shader);
			int SetGeometryShader(Engine::GeometryShader& shader);
			int SetFragmentShader(Engine::FragmentShader& shader);

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

			// locations of shader attributes
			std::vector<std::pair<Engine::Shader::ShaderAttribute, unsigned int>> attributeLocations;
            // locations of shader uniform buffers
            std::vector<std::pair<Engine::Shader::UniformBuffer, unsigned int>> UniformBufferLocations;

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
            unsigned int volumeMapLocation;
            unsigned int shadowMapsLocation;

            // shader block locations
            static unsigned int mvpBlock;
            static unsigned int materialParametersBlock;
            static unsigned int pointLightsBLock;
            static unsigned int spotLightsBLock;
            static unsigned int directionalLightsBLock;
            static unsigned int ambientLightsBLock;

            Engine::Shader::ShaderFlag shaderFlags;
            static Shader* currentShader;
    };

}

#endif //  SHADER_H
