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
#include "vector"
#include "utility"

namespace Engine
{
    // base to be inherited by all OpenGL shaders
    class Shader
    {
	public:
        Shader();
        ~Shader();

        void AddSource(std::string filePath);

		char* GetCompileLog();
		unsigned int GetShader();
        bool GetCompilatonStatus();

	protected:
        virtual void SetShaderType() = 0;

		char compileLog[512];
		unsigned int shader;
        bool compilationStatus;
    };

    // ===== Shader implementations ======

    // OpenGL vertex shader
    class VertexShader : public Engine::Shader
    {
        
    public:

    private:
        void SetShaderType() override;
    };

    // OpenGL geometry shader
    class GeometryShader : public Engine::Shader
    {
        using Engine::Shader::Shader;

	public:

    private:
        void SetShaderType() override;
    };

    // OpenGL fragment shader
    class FragmentShader : public Engine::Shader
    {
        using Engine::Shader::Shader;

	public:

    private:
        void SetShaderType() override;
    };

    // OpenGL compute shader
    class ComputeShader : public Engine::Shader
    {
        using Engine::Shader::Shader;

	public:

    private:
        void SetShaderType() override;
    };

    // OpenGL shader program wrapper class
    // encapsulates vertex, geometry and fragment shaders as well as the shader program that binds them
    // performs compiling and linking of shader programs
    // holds the internal OpenGL locations of shader attribute and buffer locations so we don't have to query the GPU for them
    // provides easy retrieval of the currently active shader program
    class ShaderProgram
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

            ShaderProgram();
            ~ShaderProgram();
            ShaderProgram(std::string vertexShader, std::string fragmentShader);
            ShaderProgram(std::string vertexShader, std::string geometryShader, std::string fragmentShader);

            // glGetCurrentProgram() is very slow, just like all functions that ask the graphics processor for data
            // so we're storing it ourselves in case we need it
            static ShaderProgram& GetCurrentShaderProgram();

            VertexShader& GetVertexShader();
            GeometryShader& GetGeometryShader();
            FragmentShader& GetFragmentShader();
            unsigned int GetProgramID();

            unsigned int GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute attribute);
            static unsigned int GetUniformBuffer(Engine::ShaderProgram::UniformBuffer buffer);

            // handle shader flags for uber shader
            Engine::ShaderProgram::ShaderFlag& GetShaderFlags();
            bool GetShaderFlag(Engine::ShaderProgram::ShaderFlag flag);
            void SetShaderFlag(Engine::ShaderProgram::ShaderFlag flag);

            std::string GetLogData();

            // load vertex, geometry and fragment shaders from files
            void SetVertexShader(std::string filePath);
            void SetGeometryShader(std::string filePath);
            void SetFragmentShader(std::string filePath);

            // load vertex, geometry and fragment shaders from objects
            void SetVertexShader(Engine::VertexShader& shader);
            void SetGeometryShader(Engine::GeometryShader& shader);
            void SetFragmentShader(Engine::FragmentShader& shader);

            // compiles vertex, geometry and fragment shaders into a program and binds it
            // needs to be run after every shader change
            int CompileProgram();

            // activates this shader program
            // needs to be run before every draw call that will make use of this shader
            void Activate();

        private:
            // necessary for normal shading pipeline
            Engine::VertexShader vertexShader;
            Engine::FragmentShader fragmentShader;

            // optional shaders
            Engine::GeometryShader* geometryShader;

            // will be set to 1 if shader compilation succeeds, 0 otherwise
            bool compileSuccess;
            char shaderProgramCompileLog[512];

			// locations of shader attributes
			std::vector<std::pair<Engine::ShaderProgram::ShaderAttribute, unsigned int>> attributeLocations;
            // locations of shader uniform buffers
            std::vector<std::pair<Engine::ShaderProgram::UniformBuffer, unsigned int>> UniformBufferLocations;

            unsigned int programID;

			// names of shader attributes inside the actual shaders, these need to match for OpenGL to be able to find their locations
			inline const static std::pair<Engine::ShaderProgram::ShaderAttribute, std::string> attributeNames[] =
			{ {ShaderAttribute::VERTEX_POSITION_LOCATION, "test"} };

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

            Engine::ShaderProgram::ShaderFlag shaderFlags;

            static ShaderProgram* currentShaderProgram;
    };

}

#endif //  SHADER_H
