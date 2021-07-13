#ifndef MODEL_H
#define MODEL_H

#define STB_IMAGE_IMPLEMENTATION

#ifndef OBJL_INCLUDED
#include "OBJ_Loader.h"
#define OBJL_INCLUDED
#endif

#include "Mesh.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glfw3.h"

#include "vector"
#include "iostream"

namespace Engine
{

	class Model
	{
	public:
		Model();
		Model(const char* filePath);

		void LoadMesh(const char* filePath);
		void Draw(Engine::Shader* shader);

	private:
		std::vector<Mesh> meshes;
		std::vector<Texture> textures;
	};

	

}

#endif