#include "Material.h"

Engine::Material::Material()
{
	diffuse = {};
	roughness = {};
	metallic = {};
}


void Engine::Material::SetDiffuse(const char* filePath)
{
	this->diffuse = Engine::Texture(filePath);
}

void Engine::Material::SetRoughness(const char* filePath)
{
	this->roughness = Engine::Texture(filePath);
}

void Engine::Material::SetMetallic(const char* filePath)
{
	this->metallic = Engine::Texture(filePath);
}

Engine::Texture Engine::Material::GetDiffuse()
{
	return this->diffuse;
}

Engine::Texture Engine::Material::GetRougness()
{
	return this->roughness;
}

Engine::Texture Engine::Material::GetMetallic()
{
	return this->metallic;
}