#include "Material.h"

Engine::Material::Material()
{
	diffuse = {};
	roughness = {};
	metallic = {};
}

Engine::Material::Material(const char* filePath)
{
	diffuse = {};
	roughness = {};
	metallic = {};
}

void Engine::Material::SetDiffuse(const char* filePath)
{
	this->diffuseMap = Engine::Texture(filePath);
}

void Engine::Material::SetDiffuse(Texture texture)
{
	this->diffuseMap = texture;
}

void Engine::Material::SetRoughness(const char* filePath)
{
	this->roughnessMap = Engine::Texture(filePath);
}

void Engine::Material::SetRoughness(Texture texture)
{
	this->roughnessMap = texture;
}

void Engine::Material::SetMetallic(const char* filePath)
{
	this->metallicMap = Engine::Texture(filePath);
}

void Engine::Material::SetMetallic(Texture texture)
{
	this->metallicMap = texture;
}

Engine::Texture* Engine::Material::GetDiffuse()
{
	return &this->diffuseMap;
}

Engine::Texture* Engine::Material::GetRougness()
{
	return &this->roughnessMap;
}

Engine::Texture* Engine::Material::GetMetallic()
{
	return &this->metallicMap;
}