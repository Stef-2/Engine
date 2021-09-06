#include "Bone.h"

Engine::Bone::Bone()
{
	this->id = {};
	this->name = {};
	this->numAffectedVerts = {};
	this->transform = {};

	this->positions = {};
	this->rotations = {};
	this->scales = {};
}

Engine::Bone::Bone(std::string name, glm::mat4 transform, unsigned int numAffectedVerts)
{
	this->id = {};
	this->name = name;
	this->numAffectedVerts = numAffectedVerts;
	this->transform = transform;

	this->positions = {};
	this->rotations = {};
	this->scales = {};
}

Engine::Bone::Bone(std::string name, glm::mat4 transform, unsigned int numAffectedVerts, unsigned int ID)
{
	this->id = ID;
	this->name = name;
	this->numAffectedVerts = numAffectedVerts;
	this->transform = transform;

	this->positions = {};
	this->rotations = {};
	this->scales = {};
}

std::string Engine::Bone::GetName()
{
	return this->name;
}

glm::mat4 Engine::Bone::GetTransform()
{
	return this->transform;
}

std::vector<glm::mat4>& Engine::Bone::GetInheritedTransforms()
{
	return this->inheritedTransforms;
}

std::vector<std::string>& Engine::Bone::GetInheritedNames()
{
	return this->inheritedNames;
}

unsigned int Engine::Bone::GetNumAffectedVertices()
{
	return this->numAffectedVerts;
}

unsigned int Engine::Bone::GetID()
{
	return this->id;
}

std::vector<Engine::VectorKeyFrame>& Engine::Bone::GetPositions()
{
	return this->positions;
}

std::vector<Engine::QuaternionKeyFrame>& Engine::Bone::GetRotations()
{
	return this->rotations;
}

std::vector<Engine::VectorKeyFrame>& Engine::Bone::GetScales()
{
	return this->scales;
}

void Engine::Bone::SetName(std::string name)
{
	this->name = name;
}

void Engine::Bone::SetTransform(glm::mat4 transform)
{
	this->transform = transform;
}

void Engine::Bone::AddInheritedTransform(glm::mat4 transform)
{
	this->inheritedTransforms.push_back(transform);
}

void Engine::Bone::AddInheritedName(std::string name)
{
	this->inheritedNames.push_back(name);
}

void Engine::Bone::SetNumAffectedVerts(unsigned int numAffectedVerts)
{
	this->numAffectedVerts = numAffectedVerts;
}

void Engine::Bone::SetID(unsigned int id)
{
	this->id = id;
}

void Engine::Bone::SetPositions(std::vector<Engine::VectorKeyFrame> positions)
{
	this->positions = positions;
}

void Engine::Bone::SetRotations(std::vector<Engine::QuaternionKeyFrame> rotations)
{
	this->rotations = rotations;
}

void Engine::Bone::SetScales(std::vector<Engine::VectorKeyFrame> scales)
{
	this->scales = scales;
}