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
#include "map"

namespace Engine
{
    // base to be inherited by all OpenGL shaders
    class Shader
    {
	public:
        Shader();
        ~Shader();

        void AddSource(std::string filePath);

		unsigned int GetShader();
        bool GetCompilatonStatus();

	protected:
        virtual void SetShaderType() = 0;

		unsigned int shader;
        bool compilationStatus;
    };

    // ===== Shader implementations ======

    // OpenGL vertex shader
    class VertexShader : public Engine::Shader
    {
        using Engine::Shader::Shader;

    private:
        void SetShaderType() override;
    };

    // OpenGL geometry shader
    class GeometryShader : public Engine::Shader
    {
        using Engine::Shader::Shader;

    private:
        void SetShaderType() override;
    };

    // OpenGL fragment shader
    class FragmentShader : public Engine::Shader
    {
        using Engine::Shader::Shader;

    private:
        void SetShaderType() override;
    };

    // OpenGL compute shader
    class ComputeShader : public Engine::Shader
    {
        using Engine::Shader::Shader;

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
            
            ShaderProgram(ShaderProgram&&) = default;
            ShaderProgram& operator= (ShaderProgram&&) = default;

            ShaderProgram(const ShaderProgram&) = default;
            ShaderProgram& operator= (const ShaderProgram&) = default;
            
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

            // shading program handle
            unsigned int programID;

			// will be set to 1 if shader compilation succeeds, 0 otherwise
			bool compileSuccess;

            // necessary for normal shading pipeline
            std::shared_ptr<Engine::VertexShader> vertexShader;
            std::shared_ptr<Engine::FragmentShader> fragmentShader;

            // optional shaders
            std::shared_ptr<Engine::GeometryShader> geometryShader;

			// locations of shader attributes
			std::map<Engine::ShaderProgram::ShaderAttribute, unsigned int> attributeLocations;
            // locations of shader uniform buffers
            static std::map<Engine::ShaderProgram::UniformBuffer, unsigned int> UniformBufferLocations;

			// names of shader attributes and uniform buffers inside the actual GLSL shaders
            // these need to match for OpenGL to be able to find their locations and send them data
			inline const static std::map<Engine::ShaderProgram::ShaderAttribute, std::string> attributeNames =
			{ 
                {ShaderAttribute::VERTEX_POSITION_LOCATION, "vertexPosition"},
                {ShaderAttribute::VERTEX_NORMAL_LOCATION, "vertexNormal"},
                {ShaderAttribute::VERTEX_UV_LOCATION, "vertexCoordinate"},

                {ShaderAttribute::BONE_TRANSFORMATIONS, "boneTransformations"},
                {ShaderAttribute::VERTEX_BONE_ID_LOCATION, "boneIDs"},
                {ShaderAttribute::VERTEX_BONE_WEIGHTS_LOCATION, "boneWeights"},

                {ShaderAttribute::MODEL_LOCATION, "model"},
                {ShaderAttribute::VIEW_LOCATION, "view"},
                {ShaderAttribute::PROJECTION_LOCATION, "projection"},

                {ShaderAttribute::SHADER_PARAMETERS, "shaderFlags"},

                {ShaderAttribute::DIFFUSE_MAP, "diffuseMap"},
                {ShaderAttribute::ROUGHNESS_MAP, "roughnessMap"},
                {ShaderAttribute::METALLIC_MAP, "metallicMap"},
                {ShaderAttribute::SPECULAR_MAP, "specularMap"},
                {ShaderAttribute::NORMAL_MAP, "normalMap"},
                {ShaderAttribute::ALPHA_MAP, "alphaMap"},
                {ShaderAttribute::CUBE_MAP, "cubeMap"},
                {ShaderAttribute::VOLUME_MAP, "volumeMap"},
                {ShaderAttribute::SHADOW_MAPS, "shadowMaps"}
            };

            inline const static std::map<Engine::ShaderProgram::UniformBuffer, std::string> uniformBufferNames =
            {
                {UniformBuffer::MVP_MATRICES, "mvpMatrices"},
                {UniformBuffer::MATERIAL_PARAMETERS, "materialParameters"},
				{UniformBuffer::POINT_LIGHTS, "PointLights"},
                {UniformBuffer::SPOT_LIGHTS, "SpotLights"},
                {UniformBuffer::DIRECTIONAL_LIGHTS, "DirectionalLights"},
                {UniformBuffer::AMBIENT_LIGHTS, "AmbientLights"}
            };

            Engine::ShaderProgram::ShaderFlag shaderFlags;

            static ShaderProgram* currentShaderProgram;
    };

}

#endif //  SHADER_H
