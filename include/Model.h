#ifndef MODEL_H
#define MODEL_H

#define STB_IMAGE_IMPLEMENTATION

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Bone.h"
#include "Node.h"
#include "Material.h"
#include "Animation.h"
#include "Skeleton.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BoundingVolume.h"
#include "glfw3.h"

#include "vector"
#include "iostream"
#include "string"

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
		void LoadMaterial(const Material& material);
		void SetBoundingBox(glm::vec3 mins, glm::vec3 maxs);
		void SetSkeleton(Engine::Skeleton& skelly);
		void AddAnimation(Engine::Animation);

		std::vector<Mesh>& GetMeshes();
		std::vector<Material>& GetMaterials();
		Engine::BoundingBox& GetBoundingBox();
		Engine::Skeleton& GetSkeleton();
		std::vector<Engine::Animation>& GetAnimations();

	private:
		// Mr.Skeltal
		Engine::Skeleton skeleton;
		std::vector<Engine::Animation> animations;

		std::vector<Mesh> meshes;
		std::vector<Material> materials;

		Engine::BoundingBox boundingBox;
	};

}

#endif