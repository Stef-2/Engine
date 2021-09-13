#include "Bone.h"

Engine::Bone::Bone()
{
	this->id = {};
	this->node = {};
	this->numAffectedVerts = {};
	this->transform = {};
	this->animationTime = {};
}

Engine::Bone::Bone(glm::mat4 transform, unsigned int numAffectedVerts, unsigned int ID)
{
	this->id = ID;
	this->node = {};
	this->numAffectedVerts = numAffectedVerts;
	this->transform = transform;
	this->animationTime = {};
}

Engine::Bone::Bone(Engine::Node& node, glm::mat4 transform, unsigned int numAffectedVerts, unsigned int ID)
{
	this->id = ID;
	*this->node = node;
	this->numAffectedVerts = numAffectedVerts;
	this->transform = transform;
	this->animationTime = {};
}

Engine::Node& Engine::Bone::GetNode()
{
	return *this->node;
}

glm::mat4 Engine::Bone::GetTransform()
{
	return this->transform;
}

glm::mat4 Engine::Bone::GetGlobalTransform()
{
	return this->transform * this->node->GetGlobalTransform();
}

glm::mat4 Engine::Bone::GetGlobalTransformAnimated()
{
	return this->node->GetGlobalTransform(this->animationTime) * this->transform;
}

unsigned int Engine::Bone::GetNumAffectedVertices()
{
	return this->numAffectedVerts;
}

unsigned int Engine::Bone::GetID() const
{
	return this->id;
}

void Engine::Bone::SetNode(Engine::Node& node)
{
	this->node = &node;
}

void Engine::Bone::SetTransform(glm::mat4 transform)
{
	this->transform = transform;
}

void Engine::Bone::SetAnimationTime(double time)
{
	this->animationTime = time;
}

void Engine::Bone::SetNumAffectedVerts(unsigned int numAffectedVerts)
{
	this->numAffectedVerts = numAffectedVerts;
}

void Engine::Bone::SetID(unsigned int id)
{
	this->id = id;
}