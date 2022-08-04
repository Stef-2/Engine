#ifndef MODEL_H
#define MODEL_H

#define STB_IMAGE_IMPLEMENTATION

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Material.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BoundingVolume.h"
#include "glfw3.h"
#include "Shared.h"

#include "vector"
#include "iostream"
#include "functional"
#include "algorithm"
#include "string"
#include "map"

namespace Engine
{
	// Model class, primarily encapsulates mesh(es)
	class Model
	{
	public:
		Model();
		Model(std::string filePath);

		void LoadMesh(std::string filePath);
		void LoadMesh(const Mesh& other);
		void LoadMesh(const AnimatedMesh& other);
		void SetBoundingBox(glm::vec3 mins, glm::vec3 maxs);

		std::vector<std::shared_ptr<Engine::Mesh>>& GetStaticMeshes();
		std::vector<std::shared_ptr<Engine::AnimatedMesh>>& GetAnimatedMeshes();
		
		Engine::BoundingBox& GetBoundingBox();

	private:
		std::vector<std::shared_ptr<Engine::Mesh>> staticMeshes;
		std::vector<std::shared_ptr<Engine::AnimatedMesh>> animatedMeshes;

		Engine::BoundingBox boundingBox;
	};

	// typedef for shared models
	using SharedModel = Engine::Shared<Engine::Model>;
}

#endif