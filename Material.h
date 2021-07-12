#ifndef MATERIAL_H
#define MATERIAL_H

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

		Texture diffuse;
		Texture roughness;
		Texture metallic;
	};

}
#endif