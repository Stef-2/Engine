#ifndef MATERIAL_H
#define MATERIAL_H

#define STB_IMAGE_IMPLEMENTATION
#define OBJL_INCLUDED
#define __glad_h_

#include "glfw3.h"
#include "Texture.h"
#include "glm/vec3.hpp"

#include "vector"
#include "iostream"

namespace Engine
{
	// a material class that combines multiple textures
	// and parameters to form a Physically Based Rendering (PBR) object
	// In case just a simple surface texture is needed, use only the diffuse channel
	class Material
	{
	public:
		Material();
		// load material from a file
		Material(const char* filePath);

		// diffuse reflection map (aka albedo or surface color)
		void SetDiffuseMap(const char* filePath);
		void SetDiffuseMap(std::string filePath);
		void SetDiffuseMap(const Texture& texture);

		// roughness map, inverse of a "glossiness" or "specular" reflection map
		void SetRoughnessMap(const char* filePath);
		void SetRoughnessMap(std::string filePath);
		void SetRoughnessMap(const Texture& texture);

		// metalic map, for physically correct results the values within should be either 1 or 0, not in between
		void SetMetallicMap(const char* filePath);
		void SetMetallicMap(std::string filePath);
		void SetMetallicMap(const Texture& texture);

		Texture& GetDiffuse();
		Texture& GetRougness();
		Texture& GetMetallic();

	private:

		//-----------------physically correct data----------------//
																  //
		Engine::Texture diffuseMap;								  //
		Engine::Texture roughnessMap;							  //
		Engine::Texture metallicMap;							  //
																  //
		// global multipliers for the channels above			  //
		// can be used as a value in case a map is not provided	  //
		glm::vec3 diffuse;										  //
		float roughness;										  //
		float metallic;											  //
																  //
		//--------------------------------------------------------// 

		//----------------physically incorrect data---------------//
																  //
		Engine::Texture specularMap;							  //
		Engine::Texture normalMap;								  //
		Engine::Texture alphaMap;								  //
															 	  //
		// global multipliers for the channels above		 	  //
		// can be used as a value in case a map is not provided	  //
		float specular;											  //
		glm::vec3 normal;										  //
		float alpha;											  //
																  //
		//--------------------------------------------------------//
	};

}
#endif