#ifndef MODEL_H
#define MODEL_H

#define STB_IMAGE_IMPLEMENTATION

#ifndef OBJL_INCLUDED
#include "OBJ_Loader.h"
#define OBJL_INCLUDED
#endif

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BoundingVolume.h"
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
		void LoadMaterial(Material material);
		void SetBoundingBox(glm::vec3 mins, glm::vec3 maxs);

		void Draw(Engine::Shader* shader);

		std::vector<Mesh>* GetMeshes();
		std::vector<Material>* GetMaterials();
		Engine::BoundingBox* GetBoundingBox();

	private:
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		Engine::BoundingBox boundingBox;
	};

}

#endif