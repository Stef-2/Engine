#include "Skeleton.h"

Engine::Skeleton::Skeleton()
{
	this->rootBone = {};
}

Engine::Skeleton::Skeleton(Engine::Bone& rootBone)
{
	this->rootBone = rootBone;
}

Engine::Bone& Engine::Skeleton::GetRootBone()
{
	return this->rootBone;
}

void Engine::Skeleton::SetRootBone(Engine::Bone& rootBone)
{
	this->rootBone = rootBone;
}