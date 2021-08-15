#ifndef SKELETON_H
#define SKELETON_H

#include "Bone.h"

namespace Engine
{

	class Skeleton
	{
	public:
		Skeleton();
		Skeleton(Engine::Bone& rootBone);

		Engine::Bone& GetRootBone();

		void SetRootBone(Engine::Bone& rootBone);

	private:
		Engine::Bone rootBone;
	};
}

#endif //SKELETON_H
