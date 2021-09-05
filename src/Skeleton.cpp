#include "Skeleton.h"

Engine::Skeleton::Skeleton()
{
	this->rootNode = {};
}

Engine::Skeleton::Skeleton(Engine::Node<Engine::Bone>& rootNode)
{
	this->rootNode = rootNode;
}

Engine::Node<Engine::Bone>& Engine::Skeleton::GetRootNode()
{
	return this->rootNode;
}

std::vector<Engine::Bone>& Engine::Skeleton::GetBones()
{
	return this->bones;
}

glm::mat4 Engine::Skeleton::GetGlobalInverseMatrix()
{
	return this->globalInverseMatrix;
}

void Engine::Skeleton::SetRootNode(Engine::Node<Engine::Bone>& rootNode)
{
	this->rootNode = rootNode;
}

void Engine::Skeleton::SetBones(std::vector<Engine::Bone> bones)
{
	this->bones = bones;
}

void Engine::Skeleton::SetGlobalInverseMatrix(glm::mat4 matrix)
{
	this->globalInverseMatrix = matrix;
}