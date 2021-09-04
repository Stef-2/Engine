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
		Skeleton(Engine::Node<Engine::Bone>& rootNode);

		Engine::Node<Engine::Bone>& GetRootNode();
		glm::mat4 GetGlobalInverseMatrix();

		void SetRootNode(Engine::Node<Engine::Bone>& rootNode);
		void SetGlobalInverseMatrix(glm::mat4 matrix);

	private:
		Engine::Node<Engine::Bone> rootNode;
		glm::mat4 globalInverseMatrix;
	};
}

#endif // SKELETON_H
