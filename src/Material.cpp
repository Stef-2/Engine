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
	this->diffuse = Engine::Texture(filePath);
}

void Engine::Material::SetDiffuse(Texture texture)
{
	this->diffuse = texture;
}

void Engine::Material::SetRoughness(const char* filePath)
{
	this->roughness = Engine::Texture(filePath);
}

void Engine::Material::SetRoughness(Texture texture)
{
	this->roughness = texture;
}

void Engine::Material::SetMetallic(const char* filePath)
{
	this->metallic = Engine::Texture(filePath);
}

void Engine::Material::SetMetallic(Texture texture)
{
	this->metallic = texture;
}

Engine::Texture* Engine::Material::GetDiffuse()
{
	return &this->diffuse;
}

Engine::Texture* Engine::Material::GetRougness()
{
	return &this->roughness;
}

Engine::Texture* Engine::Material::GetMetallic()
{
	return &this->metallic;
}