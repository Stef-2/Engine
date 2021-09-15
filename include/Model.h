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

#include "vector"
#include "iostream"
#include "functional"
#include "algorithm"
#include "string"
#include "map"

namespace Engine
{
	// Model class, primarely encapsulates mesh(es) and material(s)
	// Is able to hold animation data for models using skeletal animation
	class Model
	{
	public:
		Model();
		Model(std::string filePath);

		void LoadMesh(std::string filePath);
		void LoadMesh(const Mesh& other);
		void LoadMesh(const AnimatedMesh& other);
		void SetBoundingBox(glm::vec3 mins, glm::vec3 maxs);

		std::vector<Mesh>& GetStaticMeshes();
		std::vector<AnimatedMesh>& GetAnimatedMeshes();
		
		Engine::BoundingBox& GetBoundingBox();

	private:
		std::vector<Mesh> staticMeshes;
		std::vector<AnimatedMesh> animatedMeshes;

		Engine::BoundingBox boundingBox;
	};

}

#endif