#ifndef SKELETON_H
#define SKELETON_H

#include "Node.h"
#include "Bone.h"

#include "glm/glm.hpp"

namespace Engine
{

	class Skeleton
	{
	public:
		Skeleton();
		Skeleton(Engine::Node<Engine::Bone> rootNode);

		Engine::Node<Engine::Bone>& GetRootNode();
		std::vector<Engine::Bone>& GetBones();
		glm::mat4 GetGlobalInverseMatrix();

		void SetRootNode(Engine::Node<Engine::Bone> rootNode);
		void SetBones(std::vector<Engine::Bone> bones);
		void SetGlobalInverseMatrix(glm::mat4 matrix);

	private:
		Engine::Node<Engine::Bone> rootNode;
		std::vector<Engine::Bone> bones;
		glm::mat4 globalInverseMatrix;
	};
}

#endif // SKELETON_H
