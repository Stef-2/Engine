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

std::string Engine::Bone::GetName()
{
	return this->name;
}

glm::mat4 Engine::Bone::GetTransform()
{
	return this->transform;
}

unsigned int Engine::Bone::GetNumAffectedVertices()
{
	return this->numAffectedVerts
}

unsigned int Engine::Bone::GetID()
{
	return this->id;
}

std::vector<Engine::PositionKey> Engine::Bone::GetPositions()
{
	return this->positions;
}

std::vector<Engine::RotationKey> Engine::Bone::GetRotations()
{
	return this->rotations;
}

std::vector<Engine::ScaleKey> Engine::Bone::GetScales()
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

void Engine::Bone::SetNumAffectedVerts(unsigned int numAffectedVerts)
{
	this->numAffectedVerts = numAffectedVerts;
}

void Engine::Bone::SetID(unsigned int id)
{
	this->id = id;
}

void Engine::Bone::SetPositions(std::vector<Engine::PositionKey> positions)
{
	this->positions = positions;
}

void Engine::Bone::SetRotations(std::vector<Engine::RotationKey> rotations)
{
	this->rotations = rotations;
}

void Engine::Bone::SetScales(std::vector<Engine::ScaleKey> scales)
{
	this->scales = scales;
}