#ifndef SKELETON_H
#define SKELETON_H

#include "Node.h"
#include "Bone.h"

#include "glm/glm.hpp"

namespace Engine
{
	// Mr. Skeltal, holding a bunch of bones and a tree of their Engine::Node(s), the root of which is accessible
	// also holds the inverse of the global transformation matrix, which needs to be used for final node / bone transformations
	class Skeleton
	{
	public:
		Skeleton();
		Skeleton(Engine::Node* rootNode);
		Skeleton(Engine::Node* rootNode, std::vector<Engine::Bone> bones);

		Engine::Node& GetRootNode();
		std::vector<Engine::Bone>& GetBones();
		glm::mat4 GetGlobalInverseMatrix();

		void SetRootNode(Engine::Node* rootNode);
		void SetBones(std::vector<Engine::Bone> bones);
		void AddBone(Engine::Bone bone);
		void SetGlobalInverseMatrix(glm::mat4 matrix);

	private:
		Engine::Node* rootNode;
		std::vector<Engine::Bone> bones;
		glm::mat4 globalInverseMatrix;
	};
}

#endif // SKELETON_H
