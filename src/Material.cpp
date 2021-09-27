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

void Engine::Material::SetNormalMap(const char* filePath)
{
	this->normalMap = Engine::Texture(filePath);
}

void Engine::Material::SetNormalMap(std::string filePath)
{
	this->normalMap = Engine::Texture(filePath);
}

void Engine::Material::SetNormalMap(const Texture& texture)
{
	this->normalMap = texture;
}

Engine::Texture& Engine::Material::GetDiffuseMap()
{
	return this->diffuseMap;
}

Engine::Texture& Engine::Material::GetRougnessMap()
{
	return this->roughnessMap;
}

Engine::Texture& Engine::Material::GetMetallicMap()
{
	return this->metallicMap;
}

Engine::Texture& Engine::Material::GetNormalMap()
{
	return this->normalMap;
}