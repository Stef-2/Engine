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

void Engine::Material::SetDiffuseMap(const char* filePath)
{
	this->diffuseMap = Engine::Texture(filePath);
}

void Engine::Material::SetDiffuseMap(std::string filePath)
{
	this->diffuseMap = Engine::Texture(filePath);
}

void Engine::Material::SetDiffuseMap(const Texture& texture)
{
	this->diffuseMap = texture;
}

void Engine::Material::SetRoughnessMap(const char* filePath)
{
	this->roughnessMap = Engine::Texture(filePath);
}

void Engine::Material::SetRoughnessMap(std::string filePath)
{
	this->diffuseMap = Engine::Texture(filePath);
}

void Engine::Material::SetRoughnessMap(const Texture& texture)
{
	this->roughnessMap = texture;
}

void Engine::Material::SetMetallicMap(const char* filePath)
{
	this->metallicMap = Engine::Texture(filePath);
}

void Engine::Material::SetMetallicMap(std::string filePath)
{
	this->diffuseMap = Engine::Texture(filePath);
}

void Engine::Material::SetMetallicMap(const Texture& texture)
{
	this->metallicMap = texture;
}

Engine::Texture& Engine::Material::GetDiffuse()
{
	return this->diffuseMap;
}

Engine::Texture& Engine::Material::GetRougness()
{
	return this->roughnessMap;
}

Engine::Texture& Engine::Material::GetMetallic()
{
	return this->metallicMap;
}