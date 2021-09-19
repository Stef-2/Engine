#include "Light.h"

// --- Light ---

glm::vec3 Engine::Light::GetColor()
{
	return this->color;
}

float Engine::Light::GetIntensity()
{
	return this->intensity;
}

void Engine::Light::SetIntensity(float intensity)
{
	this->intensity = intensity;
}

void Engine::Light::SetColor(glm::vec3 color)
{
	this->color = color;
}

// --- Physical Light ---

float Engine::PhysicalLight::GetIntensity(glm::vec3 atPosition)
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

std::vector<Engine::PointLight::Data*> Engine::PointLight::lights = {};

Engine::PointLight::PointLight() : data(*this)
{
	this->position = { 0.0f, 1.0f, 0.0f };
	this->color = { 0.0f, 1.0f, 0.0f };
	this->intensity = { 1000.0f };

	// add ourselves to the stack
	Engine::PointLight::lights.push_back(&this->data);

	// check if the shader storage buffer is initialized
	// it may not be if lights are added before any shader that uses them is initialized first
	//if (int(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffers::POINT_LIGHTS)) >= 0)
	
	struct PointLightData
	{
		glm::vec4 position;
		glm::vec4 color;
		float intensity;
	}guy;

	guy.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	guy.position = { 0.0f, 20.0f, 0.0f, 0.0f };
	guy.intensity = 50000;

	// bind and refill the buffer with the expanded light vector
	std::cout << "buffer id: " << Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffers::POINT_LIGHTS) << std::endl;
	//std::cout << "sizeof: PointLight::Data: " << sizeof(PointLightData) << ", sizeof: this->lights[0]: " << sizeof(this->lights.at(0)) << ", sizeof: Full: " << sizeof(PointLightData) << std::endl;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffers::POINT_LIGHTS));

	glNamedBufferStorage(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffers::POINT_LIGHTS),
		this->lights.size() * sizeof(guy), NULL, GL_DYNAMIC_STORAGE_BIT);

	//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Engine::PointLight::Data), NULL, GL_DYNAMIC_DRAW);

	for (size_t i = 0; i < this->lights.size(); i++)
	{
		glNamedBufferSubData(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffers::POINT_LIGHTS), i * sizeof(guy), sizeof(guy), &guy);
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	
}

Engine::PointLight::~PointLight()
{
	// find and remove ourselves from the light stack
	for (size_t i = 0; i < Engine::PointLight::GetLights().size(); i++)
		if (Engine::PointLight::GetLights().at(i) == &this->data)
		{
			Engine::PointLight::GetLights().erase(Engine::PointLight::GetLights().begin() + i);
			Engine::PointLight::GetLights().shrink_to_fit();

			if (Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffers::POINT_LIGHTS))
			{
				// bind and refill the buffer with the reduced light vector
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffers::POINT_LIGHTS));
				glNamedBufferStorage(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffers::POINT_LIGHTS),
					this->lights.size() * sizeof(Engine::PointLight::data), &this->lights, GL_DYNAMIC_STORAGE_BIT);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			}

			break;
		}
}

std::vector<Engine::PointLight::Data*>& Engine::PointLight::GetLights()
{
	return Engine::PointLight::lights;
}

Engine::PointLight::Data::Data(Engine::PointLight& outter) : position(outter.position), color(outter.color), intensity(outter.intensity)
{
	// required for an initializer list of reference members
}

void* Engine::PointLight::GetData()
{
	return &this->data;
}

// --- Spot Light ---

std::vector<Engine::SpotLight::Data*> Engine::SpotLight::lights = {};

Engine::SpotLight::SpotLight() : data(*this)
{
	this->position = {};
	this->color = {};
	this->intensity = {};


}

std::vector<Engine::SpotLight::Data*>& Engine::SpotLight::GetLights()
{
	return Engine::SpotLight::lights;
}

Engine::SpotLight::Data::Data(Engine::SpotLight& outter) : position(outter.position), rotation(outter.rotation), color(outter.color), intensity(outter.intensity), angle(45.0)
{
	// required for an initializer list of reference members
}

void* Engine::SpotLight::GetData()
{
	return &this->data;
}

// --- Directional Light ---

std::vector<Engine::DirectionalLight::Data*> Engine::DirectionalLight::lights = {};

Engine::DirectionalLight::DirectionalLight() : data(*this)
{
	this->rotation = {};
	this->color = {};
	this->intensity = {};
}

std::vector<Engine::DirectionalLight::Data*>& Engine::DirectionalLight::GetLights()
{
	return Engine::DirectionalLight::lights;
}

glm::vec3 Engine::DirectionalLight::GetRotation()
{
	return this->rotation;
}

void Engine::DirectionalLight::SetRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

Engine::DirectionalLight::Data::Data(Engine::DirectionalLight& outter) : rotation(outter.rotation), color(outter.color), intensity(outter.intensity)
{
	// required for an initializer list of reference members
}

void* Engine::DirectionalLight::GetData()
{
	return &this->data;
}

// --- Ambient Light ---

std::vector<Engine::AmbientLight::Data*> Engine::AmbientLight::lights = {};

Engine::AmbientLight::AmbientLight() : data(*this)
{
	this->position = {};
	this->color = {};
	this->intensity = {};
}

std::vector<Engine::AmbientLight::Data*>& Engine::AmbientLight::GetLights()
{
	return Engine::AmbientLight::lights;
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
	// required for an initializer list of reference members
}

void* Engine::AmbientLight::GetData()
{
	return &this->data;
}