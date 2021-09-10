#include "Skeleton.h"

Engine::Skeleton::Skeleton()
{
	this->rootNode = {};
	this->bones = {};
}

Engine::Skeleton::Skeleton(Engine::Node* rootNode)
{
	this->rootNode = rootNode;
	this->bones = {};
}

Engine::Skeleton::Skeleton(Engine::Node* rootNode, std::vector<Engine::Bone> bones)
{
	this->rootNode = rootNode;
	this->bones = bones;
}

Engine::Node& Engine::Skeleton::GetRootNode()
{
	return *this->rootNode;
}

std::vector<Engine::Bone>& Engine::Skeleton::GetBones()
{
	return this->bones;
}

glm::mat4 Engine::Skeleton::GetGlobalInverseMatrix()
{
	return this->globalInverseMatrix;
}

glm::mat4 Engine::Skeleton::GetFinalBoneTransform(unsigned int index)
{
	return this->globalInverseMatrix * this->bones.at(index).GetGlobalTransform();
}

glm::mat4 Engine::Skeleton::GetFinalBoneTransformAnimated(unsigned int index)
{
	return this->globalInverseMatrix * this->bones.at(index).GetGlobalTransformAnimated();
}

void Engine::Skeleton::SetBones(std::vector<Engine::Bone> bones)
{
	this->bones = bones;
}

void Engine::Skeleton::AddBone(Engine::Bone bone)
{
	this->bones.push_back(bone);
}

void Engine::Skeleton::SetRootNode(Engine::Node* rootNode)
{
	this->rootNode = rootNode;
}

void Engine::Skeleton::SetGlobalInverseMatrix(glm::mat4 matrix)
{
	this->globalInverseMatrix = matrix;
}