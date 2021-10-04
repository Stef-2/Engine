#include "Light.h"

//=============================================================================
// -------------------------------- Light -------------------------------------
//=============================================================================

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
	this->UpdateLight();
}

void Engine::Light::SetColor(glm::vec3 color)
{
	this->color = color;
	this->UpdateLight();
}

//=============================================================================
// ---------------------------- Physical Light --------------------------------
//=============================================================================

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

void Engine::PhysicalLight::MoveRelative(glm::vec3 direction, float intensity)
{
	Engine::Object::MoveRelative(direction, intensity);

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::PhysicalLight::MoveRelative(float x, float y, float z)
{
	Engine::Object::MoveRelative(x, y, z);

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::PhysicalLight::MoveAbsolute(float x, float y, float z)
{
	Engine::Object::MoveAbsolute(x, y, z);

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::PhysicalLight::RotateRelative(float x, float y, float z)
{
	Engine::Object::RotateRelative(x, y, z);

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::PhysicalLight::RotateAbsolute(float x, float y, float z)
{
	Engine::Object::RotateAbsolute(x, y, z);

	// update the light stack on the GPU
	this->UpdateLight();
}

//=============================================================================
// ----------------------------- Point Light ----------------------------------
//=============================================================================

std::vector<Engine::PointLight*> Engine::PointLight::lights = {};

Engine::PointLight::PointLight()
{
	this->position = {0.0f, 0.0f, 0.0f};
	this->color = {1.0f, 1.0f, 1.0f};
	this->intensity = {};

	// add ourselves to the stack
	Engine::PointLight::lights.push_back(this);

	// update the light stack on the GPU
	this->UpdateLights();
}

Engine::PointLight::~PointLight()
{
	// find and remove ourselves from the light stack
	for (size_t i = 0; i < Engine::PointLight::GetLights().size(); i++)
		if (Engine::PointLight::GetLights().at(i) == this)
		{
			Engine::PointLight::GetLights().erase(Engine::PointLight::GetLights().begin() + i);
			Engine::PointLight::GetLights().shrink_to_fit();

			break;
		}

	// update the light stack on the GPU
	this->UpdateLights();
}

std::vector<Engine::PointLight*>& Engine::PointLight::GetLights()
{
	return Engine::PointLight::lights;
}

float Engine::PointLight::GetIntensityAt(glm::vec3 atPosition)
{
	// physical light decay is proportional to inverse of the square of the distance between the source and point X
	return this->intensity * glm::pow(glm::distance(this->position, atPosition), 2);
}

float Engine::PointLight::GetEffectiveRadius()
{
	// given our current intensity and the inverse square intensity decay law, calculate the distance past which this lights contribution can be discarded
	// threshold is an arbitrary value that we consider to no longer provide a significant contribution to final lighting output
	float threshold = 100;
	return glm::sqrt(this->intensity / threshold);
}

void Engine::PointLight::UpdateLight()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::POINT_LIGHTS)) >= 0)
	{
		// GLSL forces a 4x4 byte allignement in data structures
		// we need to convert our vec3s into vec4s and temporarily make an actual data object to be passed to the shader

		struct PointLightData
		{
			glm::vec4 position;
			glm::vec4 color;
			float intensity;
			float vec4padding[3];
		};

		auto ToVec4Allignement = [](Engine::PointLight& data) -> PointLightData {
			return { glm::vec4(data.position, 0.0f), glm::vec4(data.color, 1.0f), data.intensity };
		};

		PointLightData pointLight = ToVec4Allignement(*this);

		// bind the buffer and replace updated light data
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::POINT_LIGHTS));

		// find ourselves in the lights stack and replace the old data
		for (size_t i = 0; i < this->lights.size(); i++)
			if (this->lights.at(i) == this) {
				glNamedBufferSubData(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::POINT_LIGHTS),
					i * sizeof(PointLightData), sizeof(PointLightData), &pointLight);
				break;
			}

		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

void Engine::PointLight::UpdateLights()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::POINT_LIGHTS)) >= 0)
	{
		// GLSL forces a 4x4 byte allignement in data structures
		// we need to convert our vec3s into vec4s and temporarily make an actual data object to be passed to the shader

		struct PointLightData
		{
			glm::vec4 position;
			glm::vec4 color;
			float intensity;
			float vec4padding[3];
		};

		auto ToVec4Allignement = [](Engine::PointLight& data) -> PointLightData {
			return { glm::vec4(data.position, 0.0f), glm::vec4(data.color, 1.0f), data.intensity };
		};

		std::vector<PointLightData> pointLights;

		for (size_t i = 0; i < this->lights.size(); i++)
			pointLights.push_back(ToVec4Allignement(*this->lights.at(i)));
		
		// bind and refill the buffer with the expanded light vector
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::POINT_LIGHTS));

		//glNamedBufferStorage(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::POINT_LIGHTS),
			//(pointLights.size()) * sizeof(PointLightData), &pointLights, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferData(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::POINT_LIGHTS), (pointLights.size()) * sizeof(PointLightData), &pointLights, GL_DYNAMIC_DRAW);
		//glBufferStorage(GL_SHADER_STORAGE_BUFFER, pointLights.size() * sizeof(PointLightData), &pointLights, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

//=============================================================================
// ---------------------------- Spot Light ------------------------------------
//=============================================================================

std::vector<Engine::SpotLight*> Engine::SpotLight::lights = {};

Engine::SpotLight::SpotLight()
{
	this->position = { 0.0f, 0.0f, 0.0f };
	this->rotation = { 0.0f, 0.0f, 0.0f };
	this->color = { 1.0f, 1.0f, 1.0f };
	this->intensity = {};
	this->angle = {};

	// add ourselves to the stack
	Engine::SpotLight::lights.push_back(this);

	// update the light stack on the GPU
	this->UpdateLights();
}

Engine::SpotLight::~SpotLight()
{
	// find and remove ourselves from the light stack
	for (size_t i = 0; i < Engine::SpotLight::GetLights().size(); i++)
		if (Engine::SpotLight::GetLights().at(i) == this)
		{
			Engine::SpotLight::GetLights().erase(Engine::SpotLight::GetLights().begin() + i);
			Engine::SpotLight::GetLights().shrink_to_fit();

			break;
		}

	// update the light stack on the GPU
	this->UpdateLights();
}

float Engine::SpotLight::GetAngle()
{
	return this->angle;
}

float Engine::SpotLight::GetSharpness()
{
	return this->sharpness;
}

float Engine::SpotLight::GetIntensityAt(glm::vec3 atPosition)
{
	// physical light decay is proportional to inverse of the square of the distance between the source and point X
	return this->intensity * glm::pow(glm::distance(this->position, atPosition), 2);
}

std::vector<Engine::SpotLight*>& Engine::SpotLight::GetLights()
{
	return Engine::SpotLight::lights;
}

void Engine::SpotLight::UpdateLight()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::SPOT_LIGHTS)) >= 0)
	{
		// GLSL forces a 4x4 byte allignement in data structures
		// we need to convert our vec3s into vec4s and temporarily make an actual data object to be passed to the shader

		struct SpotLightData
		{
			glm::vec4 position;
			glm::vec4 orientation;
			glm::vec4 color;
			float intensity;
			float angle;
			float sharpness;
			float vec4padding[1];
		};

		auto ToVec4Allignement = [](Engine::SpotLight& data) -> SpotLightData {
			return { glm::vec4(data.position, 0.0f), glm::vec4(data.rotation, 1.0f), glm::vec4(data.color, 1.0f), data.intensity, data.angle, data.sharpness };
		};

		SpotLightData spotLight = ToVec4Allignement(*this);

		// bind the buffer and replace updated light data
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::SPOT_LIGHTS));

		// find ourselves in the lights stack and replace the old data
		#pragma omp simd
		for (size_t i = 0; i < this->lights.size(); i++)
			if (this->lights.at(i) == this) {
				glNamedBufferSubData(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::SPOT_LIGHTS),
					i * sizeof(SpotLightData), sizeof(SpotLightData), &spotLight);
				break;
			}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

void Engine::SpotLight::UpdateLights()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::SPOT_LIGHTS)) >= 0)
	{
		// GLSL forces a 4x4 byte allignement in data structures
		// we need to convert our vec3s into vec4s and temporarily make an actual data object to be passed to the shader

		struct SpotLightData
		{
			glm::vec4 position;
			glm::vec4 rotation;
			glm::vec4 color;
			float intensity;
			float angle;
			float sharpness;
			float vec4padding[1];
		};

		auto ToVec4Allignement = [](Engine::SpotLight& data) -> SpotLightData {
			return { glm::vec4(data.position, 0.0f), glm::vec4(data.rotation, 0.0f), glm::vec4(data.color, 1.0f), data.intensity, data.angle, data.sharpness };
		};

		std::vector<SpotLightData> spotLights;

		for (size_t i = 0; i < this->lights.size(); i++)
			spotLights.push_back(ToVec4Allignement(*this->lights.at(i)));

		// bind and refill the buffer with the expanded light vector
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::SPOT_LIGHTS));

		glNamedBufferStorage(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::SPOT_LIGHTS),
			this->lights.size() * sizeof(SpotLightData), &spotLights[0], GL_DYNAMIC_STORAGE_BIT);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

void Engine::SpotLight::SetAngle(float angle)
{
	this->angle = angle;

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::SpotLight::SetSharpness(float sharpness)
{
	this->sharpness = sharpness;

	// update the light stack on the GPU
	this->UpdateLight();
}

//=============================================================================
// --------------------------- Directional Light ------------------------------
//=============================================================================

std::vector<Engine::DirectionalLight*> Engine::DirectionalLight::lights = {};

Engine::DirectionalLight::DirectionalLight()
{
	this->orientation = {0.0f, 1.0f, 0.0f};
	this->color = {1.0f, 1.0f, 1.0f};
	this->intensity = {};

	// add ourselves to the stack
	Engine::DirectionalLight::lights.push_back(this);

	// update the light stack on the GPU
	this->UpdateLights();
}

Engine::DirectionalLight::~DirectionalLight()
{
	// find and remove ourselves from the light stack
	for (size_t i = 0; i < Engine::DirectionalLight::GetLights().size(); i++)
		if (Engine::DirectionalLight::GetLights().at(i) == this)
		{
			Engine::DirectionalLight::GetLights().erase(Engine::DirectionalLight::GetLights().begin() + i);
			Engine::DirectionalLight::GetLights().shrink_to_fit();

			break;
		}

	// update the light stack on the GPU
	this->UpdateLights();
}

std::vector<Engine::DirectionalLight*>& Engine::DirectionalLight::GetLights()
{
	return Engine::DirectionalLight::lights;
}

glm::vec3 Engine::DirectionalLight::GetPosition()
{
	return this->position;
}

glm::vec3 Engine::DirectionalLight::GetOrientation()
{
	return this->orientation;
}

void Engine::DirectionalLight::SetPosition(glm::vec3 position)
{
	this->position = position;

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::DirectionalLight::SetOrientation(glm::vec3 orientation)
{
	this->orientation = orientation;

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::DirectionalLight::UpdateLight()
{
	// GLSL forces a 4x4 byte allignement in data structures
		// we need to convert our vec3s into vec4s and temporarily make an actual data object to be passed to the shader

	struct DirectionalLightData
	{
		glm::vec4 position;
		glm::vec4 orientation;
		glm::vec4 color;
		float intensity;
		float vec4padding[3];
	};

	auto ToVec4Allignement = [](Engine::DirectionalLight& data) -> DirectionalLightData {
		return { glm::vec4(data.position, 1.0f), glm::vec4(data.orientation, 1.0f), glm::vec4(data.color, 1.0f), data.intensity };
	};

	DirectionalLightData directionalLight = ToVec4Allignement(*this);

	// bind the buffer and replace updated light data
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::DIRECTIONAL_LIGHTS));

	// find ourselves in the lights stack and replace the old data
	#pragma omp simd
	for (size_t i = 0; i < this->lights.size(); i++)
		if (this->lights.at(i) == this) {
			glNamedBufferSubData(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::DIRECTIONAL_LIGHTS),
				i * sizeof(DirectionalLightData), sizeof(DirectionalLightData), &directionalLight);
			break;
		}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Engine::DirectionalLight::UpdateLights()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::DIRECTIONAL_LIGHTS)) >= 0)
	{
		// GLSL forces a 4x4 byte allignement in data structures
		// we need to convert our vec3s into vec4s and temporarily make an actual data object to be passed to the shader

		struct DirectionalLightData
		{
			glm::vec4 position;
			glm::vec4 orientation;
			glm::vec4 color;
			float intensity;
			float vec4padding[3];
		};

		auto ToVec4Allignement = [](Engine::DirectionalLight& data) -> DirectionalLightData {
			return { glm::vec4(data.position, 1.0f), glm::vec4(data.orientation, 1.0f), glm::vec4(data.color, 1.0f), data.intensity };
		};

		std::vector<DirectionalLightData> directionalLights;

		for (size_t i = 0; i < this->lights.size(); i++)
			directionalLights.push_back(ToVec4Allignement(*this->lights.at(i)));

		// bind and refill the buffer with the expanded light vector
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::DIRECTIONAL_LIGHTS));

		glNamedBufferStorage(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::DIRECTIONAL_LIGHTS),
			(this->lights.size()) * sizeof(DirectionalLightData), &directionalLights[0], GL_DYNAMIC_STORAGE_BIT);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

//=============================================================================
// ---------------------------- Ambient Light ---------------------------------
//=============================================================================

std::vector<Engine::AmbientLight*> Engine::AmbientLight::lights = {};

Engine::AmbientLight::AmbientLight()
{
	this->position = {};
	this->color = { 1.0f, 1.0f, 1.0f };
	this->intensity = {};

	// add ourselves to the stack
	Engine::AmbientLight::lights.push_back(this);

	// update the light stack on the GPU
	this->UpdateLights();
}

Engine::AmbientLight::~AmbientLight()
{
	// find and remove ourselves from the light stack
	for (size_t i = 0; i < Engine::AmbientLight::GetLights().size(); i++)
		if (Engine::AmbientLight::GetLights().at(i) == this)
		{
			Engine::AmbientLight::GetLights().erase(Engine::AmbientLight::GetLights().begin() + i);
			Engine::AmbientLight::GetLights().shrink_to_fit();

			break;
		}

	// update the light stack on the GPU
	this->UpdateLights();
}

std::vector<Engine::AmbientLight*>& Engine::AmbientLight::GetLights()
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

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::AmbientLight::UpdateLight()
{
	// GLSL forces a 4x4 byte allignement in data structures
		// we need to convert our vec3s into vec4s and temporarily make an actual data object to be passed to the shader

	struct AmbientLightData
	{
		glm::vec4 position;
		glm::vec4 color;
		float intensity;
		float vec4padding[3];
	};

	auto ToVec4Allignement = [](Engine::AmbientLight& data) -> AmbientLightData {
		return { glm::vec4(data.position, 1.0f), glm::vec4(data.color, 1.0f), data.intensity };
	};

	AmbientLightData ambientLight = ToVec4Allignement(*this);

	// bind the buffer and replace updated light data
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::AMBIENT_LIGHTS));

	// find ourselves in the lights stack and replace the old data
	#pragma omp simd
	for (size_t i = 0; i < this->lights.size(); i++)
		if (this->lights.at(i) == this) {
			glNamedBufferSubData(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::AMBIENT_LIGHTS),
				i * sizeof(AmbientLightData), sizeof(AmbientLightData), &ambientLight);
			break;
		}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Engine::AmbientLight::UpdateLights()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::AMBIENT_LIGHTS)) >= 0)
	{
		// GLSL forces a 4x4 byte allignement in data structures
		// we need to convert our vec3s into vec4s and temporarily make an actual data object to be passed to the shader

		struct AmbientLightData
		{
			glm::vec4 position;
			glm::vec4 color;
			float intensity;
			float vec4padding[3];
		};

		auto ToVec4Allignement = [](Engine::AmbientLight& data) -> AmbientLightData {
			return { glm::vec4(data.position, 1.0f), glm::vec4(data.color, 1.0f), data.intensity };
		};

		std::vector<AmbientLightData> ambientLights;

		for (size_t i = 0; i < this->lights.size(); i++)
			ambientLights.push_back(ToVec4Allignement(*this->lights.at(i)));

		// bind and refill the buffer with the expanded light vector
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::AMBIENT_LIGHTS));

		glNamedBufferStorage(Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer::AMBIENT_LIGHTS),
			this->lights.size() * sizeof(AmbientLightData), &ambientLights[0], GL_DYNAMIC_STORAGE_BIT);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}