#include "Bone.h"

Engine::Bone::Bone()
{
	this->id = {};
	this->name = {};
	this->numAffectedVerts = {};
	this->transform = {};

	this->positionKeyFrames = {};
	this->rotationKeyFrames = {};
	this->scaleKeyFrames = {};
}

Engine::Bone::Bone(std::string name, glm::mat4 transform, unsigned int numAffectedVerts)
{
	this->id = {};
	this->name = name;
	this->numAffectedVerts = numAffectedVerts;
	this->transform = transform;

	this->positionKeyFrames = {};
	this->rotationKeyFrames = {};
	this->scaleKeyFrames = {};
}

Engine::Bone::Bone(std::string name, glm::mat4 transform, unsigned int numAffectedVerts, unsigned int ID)
{
	this->id = ID;
	this->name = name;
	this->numAffectedVerts = numAffectedVerts;
	this->transform = transform;

	this->positionKeyFrames = {};
	this->rotationKeyFrames = {};
	this->scaleKeyFrames = {};
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
	return this->numAffectedVerts;
}

unsigned int Engine::Bone::GetID()
{
	return this->id;
}

std::vector<Engine::VectorKeyFrame>& Engine::Bone::GetPositionKeyFrames()
{
	return this->positionKeyFrames;
}

std::vector<Engine::QuaternionKeyFrame>& Engine::Bone::GetRotationKeyFrames()
{
	return this->rotationKeyFrames;
}

std::vector<Engine::VectorKeyFrame>& Engine::Bone::GetScaleKeyFrames()
{
	return this->scaleKeyFrames;
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

void Engine::Bone::SetPositionKeyFrames(std::vector<Engine::VectorKeyFrame> positions)
{
	this->positionKeyFrames = positions;
}

void Engine::Bone::SetRotationKeyFrames(std::vector<Engine::QuaternionKeyFrame> rotations)
{
	this->rotationKeyFrames = rotations;
}

void Engine::Bone::SetScaleKeyFrames(std::vector<Engine::VectorKeyFrame> scales)
{
	this->scaleKeyFrames = scales;
}