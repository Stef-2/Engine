#include "Volume.h"

Engine::Volume::Volume()
{
	this->mesh = {};
	this->shader = {};
	this->texture = {};
}

Engine::Volume::~Volume()
{
	// todo
}

Engine::Mesh& Engine::Volume::GetMesh()
{
	return *this->mesh;
}

Engine::ShaderProgram& Engine::Volume::GetShader()
{
	return *this->shader;
}

Engine::Texture3D& Engine::Volume::GetTexture()
{
	return *this->texture;
}

void Engine::Volume::SetMesh(Engine::Mesh& mesh)
{
	this->mesh = std::make_shared<Engine::Mesh>(mesh);
}

void Engine::Volume::SetShader(Engine::ShaderProgram& shader)
{
	this->shader = std::make_shared<Engine::ShaderProgram>(shader);
}

void Engine::Volume::SetTexture(Engine::Texture3D& texture)
{
	this->texture = std::make_shared<Engine::Texture3D>(texture);
}
