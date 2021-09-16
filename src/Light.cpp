#include "Light.h"

// --- Light ---

glm::vec3 Engine::Light::GetColor()
{
	return this->color;
}

double Engine::Light::GetIntensity()
{
	return this->intensity;
}

void Engine::Light::SetIntensity(double intensity)
{
	this->intensity = intensity;
}

void Engine::Light::SetColor(glm::vec3 color)
{
	this->color = color;
}

// --- Physical Light ---

double Engine::PhysicalLight::GetIntensity(glm::vec3 atPosition)
{
	// physical light decay is proportional to inverse of the square of the distance between the source and point X
	return this->intensity * glm::pow(glm::distance(this->position, atPosition), 2);
}

Engine::Mesh& Engine::PhysicalLight::GetMesh()
{
	return this->mesh;
}

Engine::Shader& Engine::PhysicalLight::GetShader()
{
	return this->shader;
}

void Engine::PhysicalLight::SetMesh(Engine::Mesh mesh)
{
	this->mesh = mesh;
}

void Engine::PhysicalLight::SetShader(Engine::Shader shader)
{
	this->shader = shader;
}

// --- Point Light ---

Engine::PointLight::PointLight() : data(*this)
{
	this->position = {};
	this->color = {};
	this->intensity = {};
}

Engine::PointLight::Data::Data(Engine::PointLight& outter) : position(outter.position), color(outter.color), intensity(outter.intensity)
{

}

void* Engine::PointLight::GetData()
{
	return &this->data;
}

// --- Spot Light ---

Engine::SpotLight::SpotLight() : data(*this)
{
	this->position = {};
	this->color = {};
	this->intensity = {};
}

Engine::SpotLight::Data::Data(Engine::SpotLight& outter) : position(outter.position), color(outter.color), intensity(outter.intensity), angle(45.0)
{

}

void* Engine::SpotLight::GetData()
{
	return &this->data;
}

// --- Directional Light ---

Engine::DirectionalLight::DirectionalLight() : data(*this)
{
	this->orientation = {};
	this->color = {};
	this->intensity = {};
}

glm::vec3 Engine::DirectionalLight::GetOrientation()
{
	return this->orientation;
}

void Engine::DirectionalLight::SetOrientation(glm::vec3 orientation)
{
	this->orientation = orientation;
}

Engine::DirectionalLight::Data::Data(Engine::DirectionalLight& outter) : orientation(outter.orientation), color(outter.color), intensity(outter.intensity)
{

}

void* Engine::DirectionalLight::GetData()
{
	return &this->data;
}

// --- Ambient Light ---

Engine::AmbientLight::AmbientLight() : data(*this)
{
	this->position = {};
	this->color = {};
	this->intensity = {};
}

glm::vec3 Engine::AmbientLight::GetPosition()
{
	return this->position;
}

void Engine::AmbientLight::SetPosition(glm::vec3 position)
{
	this->position = position;
}

Engine::AmbientLight::Data::Data(Engine::AmbientLight& outter) : position(outter.position), color(outter.color), intensity(outter.intensity)
{

}

void* Engine::AmbientLight::GetData()
{
	return &this->data;
}