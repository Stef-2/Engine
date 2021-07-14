#ifndef MATERIAL_H
#define MATERIAL_H

#define STB_IMAGE_IMPLEMENTATION
#define OBJL_INCLUDED
#define __glad_h_

#include "glfw3.h"
#include "Texture.h"

#include "vector"
#include "iostream"

namespace Engine
{

	class Material
	{
	public:
		Material();
		Material(const char* filePath);

		void SetDiffuse(const char* filePath);
		void SetDiffuse(Texture texture);

		void SetRoughness(const char* filePath);
		void SetRoughness(Texture texture);

		void SetMetallic(const char* filePath);
		void SetMetallic(Texture texture);

		Texture* GetDiffuse();
		Texture* GetRougness();
		Texture* GetMetallic();

	private:
		Texture diffuse;
		Texture roughness;
		Texture metallic;
	};

}
#endif