#ifndef MATERIAL_H
#define MATERIAL_H

#define STB_IMAGE_IMPLEMENTATION
#define OBJL_INCLUDED

#ifndef __glad_h_
#include "glad/glad.h"
#define __glad_h_
#endif

#include "glfw3.h"
#include "Shader.h"
#include "Texture2D.h"
#include "glm/vec3.hpp"

#include "vector"
#include "iostream"

namespace Engine
{
	// struct that matches the shader material parameters uniform buffer object
	// to be used as sizeof() and offsetof() reference in data alignment and size matching
	struct MaterialParametersReference
	{
		glm::vec3 diffuse;
		float roughness;
		float metallic;

		float specular;
		glm::vec3 normal;
		float alpha;
	};

	// a material class that combines multiple textures
	// and parameters to form a Physically Based Rendering (PBR) object
	// In case just a simple surface texture is needed, use only the diffuse channel
	class Material
	{
	public:
		Material();
		// load material from a file
		Material(const char* filePath);

		// upload and activate all texture handles of this material to OpenGL; to be used before rendering a mesh
		void Activate();
		void Activate(Engine::ShaderProgram& shader);

		// diffuse reflection map (aka albedo or surface color)
		void SetDiffuseMap(const char* filePath);
		void SetDiffuseMap(std::string filePath);
		void SetDiffuseMap(const Texture2D& texture);

		// roughness map, inverse of a "glossiness" or "specular" reflection map
		void SetRoughnessMap(const char* filePath);
		void SetRoughnessMap(std::string filePath);
		void SetRoughnessMap(const Texture2D& texture);

		// metalic map, for physically correct results the values within should be either 1 or 0, not in between
		void SetMetallicMap(const char* filePath);
		void SetMetallicMap(std::string filePath);
		void SetMetallicMap(const Texture2D& texture);

		// normal map, alters the surface normals, affecting the way light interacts with it
		void SetNormalMap(const char* filePath);
		void SetNormalMap(std::string filePath);
		void SetNormalMap(const Texture2D& texture);

		Texture2D& GetDiffuseMap();
		Texture2D& GetRougnessMap();
		Texture2D& GetMetallicMap();

		Texture2D& GetSpecularMap();
		Texture2D& GetNormalMap();
		Texture2D& GetAlphaMap();

	private:

		//-----------------physically correct data----------------
																  
		std::shared_ptr<Engine::Texture2D> diffuseMap;			  
		std::shared_ptr<Engine::Texture2D> roughnessMap;		  
		std::shared_ptr<Engine::Texture2D> metallicMap;			  
																  
		// global multipliers for the channels above			  
		// can be used as a value in case a map is not provided	  
		glm::vec3 diffuse;										  
		float roughness;										  
		float metallic;											  
																  
		//-------------------------------------------------------- 

		//----------------physically incorrect data---------------
																  
		std::shared_ptr<Engine::Texture2D> specularMap;							  
		std::shared_ptr<Engine::Texture2D> normalMap;							  
		std::shared_ptr<Engine::Texture2D> alphaMap;								  
															 	  
		// global multipliers for the channels above		 	  
		// can be used as a value in case a map is not provided	  
		float specular;											  
		glm::vec3 normal;										  
		float alpha;											  
																  
		//--------------------------------------------------------
	};

}
#endif