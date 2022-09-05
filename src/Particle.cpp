#include "Particle.h"

std::vector<Engine::Particle*> Engine::Particle::particles;

Engine::SharedStaticMesh Engine::Particle::vertexMesh;
Engine::SharedStaticMesh Engine::Particle::triangleMesh;
Engine::SharedStaticMesh Engine::Particle::quadMesh;
Engine::SharedStaticMesh Engine::Particle::cubeMesh;
Engine::SharedStaticMesh Engine::Particle::sphereMesh;

Engine::Particle::Particle(glm::vec3 position, float lifeTime)
{
	this->position = position;
	this->mementoMori = glfwGetTime() + lifeTime;

	// add us to the particle stack
	Engine::Particle::particles.push_back(this);
}

Engine::Particle::~Particle()
{
	// remove us from the particle stack
	Engine::Particle::particles.erase
	(std::find(Engine::Particle::particles.begin(), Engine::Particle::particles.end(), this));
}

std::vector<Engine::Particle*>& Engine::Particle::GetActiveParticles()
{
	return Engine::Particle::particles;
}

Engine::SharedStaticMesh Engine::Particle::GetVertexMesh()
{
	return Engine::Particle::vertexMesh;
}

Engine::SharedStaticMesh Engine::Particle::GetTriangleMesh()
{
	return Engine::Particle::triangleMesh;
}

Engine::SharedStaticMesh Engine::Particle::GetQuadMesh()
{
	return Engine::Particle::quadMesh;
}

Engine::SharedStaticMesh Engine::Particle::GetCubeMesh()
{
	return Engine::Particle::cubeMesh;
}

Engine::SharedStaticMesh Engine::Particle::GetSphereMesh()
{
	return Engine::Particle::sphereMesh;
}

void Engine::Particle::MoveRelative(glm::vec3 distance)
{
	this->position += distance;
}

void Engine::Particle::MoveAbsolute(glm::vec3 newPos)
{
	this->position = newPos;
}

float Engine::Particle::GetDeathTime()
{
	return this->mementoMori;
}
