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

		inline Engine::Node& GetRootNode();
		inline std::vector<Engine::Bone>& GetBones();
		inline glm::mat4 GetGlobalInverseMatrix();

		inline glm::mat4 GetFinalBoneTransform(unsigned int index);
		inline glm::mat4 GetFinalBoneTransformAnimated(unsigned int index);

		inline void SetRootNode(Engine::Node* rootNode);
		inline void SetBones(std::vector<Engine::Bone> bones);
		inline void AddBone(Engine::Bone bone);
		inline void SetGlobalInverseMatrix(glm::mat4 matrix);

	private:
		Engine::Node* rootNode;
		std::vector<Engine::Bone> bones;
		glm::mat4 globalInverseMatrix;
	};
}

#endif // SKELETON_H
