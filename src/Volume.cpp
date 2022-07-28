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
	return this->mesh;
}

Engine::Shader& Engine::Volume::GetShader()
{
	return this->shader;
}

Engine::Texture3D& Engine::Volume::GetTexture()
{
	return this->texture;
}

void Engine::Volume::SetMesh(Engine::Mesh& mesh)
{
	this->mesh = mesh;
}

void Engine::Volume::SetShader(Engine::Shader& shader)
{
	this->shader = shader;
}

void Engine::Volume::SetTexture(Engine::Texture3D& texture)
{
	this->texture = texture;
}
