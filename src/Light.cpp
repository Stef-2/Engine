#include "Light.h"

// ============================================================================
// -------------------------------- Light -------------------------------------
// ============================================================================

glm::vec3 Engine::Light::GetColor() const
{
	return this->color;
}

float Engine::Light::GetIntensity() const
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

// ============================================================================
// ---------------------------- Physical Light --------------------------------
// ============================================================================

Engine::Mesh& Engine::PhysicalLight::GetMesh()
{
	return *this->mesh;
}

Engine::ShaderProgram& Engine::PhysicalLight::GetShader()
{
	return *this->shader;
}

float Engine::PhysicalLight::GetEffectiveRadius() const
{
	return this->effectiveRadius;
}

void Engine::PhysicalLight::SetMesh(Engine::Mesh mesh)
{
	this->mesh = std::make_shared<Engine::Mesh>(mesh);
}

void Engine::PhysicalLight::SetShader(Engine::ShaderProgram shader)
{
	this->shader = std::make_shared<Engine::ShaderProgram>(shader);
}

void Engine::PhysicalLight::SetIntensity(float intensity)
{
	this->intensity = intensity;

	// given our current intensity and the inverse square intensity decay law, calculate the distance past which this lights contribution can be discarded
	// threshold is an arbitrary value that we consider to no longer provide a significant contribution to final lighting output
	this->effectiveRadius = glm::sqrt(this->intensity / Engine::PhysicalLight::threshold);
}

void Engine::PhysicalLight::MoveRelative(glm::vec3 direction, float intensity)
{
	Engine::Object::MoveRelative(direction, intensity);
	this->SetView();

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::PhysicalLight::MoveRelative(float x, float y, float z)
{
	Engine::Object::MoveRelative(x, y, z);
	this->SetView();

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::PhysicalLight::MoveAbsolute(float x, float y, float z)
{
	Engine::Object::MoveAbsolute(x, y, z);
	this->SetView();

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::PhysicalLight::RotateRelative(float x, float y, float z)
{
	Engine::Object::RotateRelative(x, y, z);
	this->SetView();

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::PhysicalLight::RotateAbsolute(float x, float y, float z)
{
	Engine::Object::RotateAbsolute(x, y, z);
	this->SetView();

	// update the light stack on the GPU
	this->UpdateLight();
}

// ============================================================================
// ----------------------------- Point Light ----------------------------------
// ============================================================================

std::vector<Engine::PointLight*> Engine::PointLight::lights = {};

Engine::PointLight::PointLight()
{
	this->position = {0.0f, 0.0f, 0.0f};
	this->color = {1.0f, 1.0f, 1.0f};
	this->intensity = {};
	this->effectiveRadius = {};
	this->SetProjection();

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

float Engine::PointLight::GetIntensityAt(glm::vec3 atPosition) const
{
	// physical light decay is proportional to inverse of the square of the distance between the source and point X
	return this->intensity * glm::pow(glm::distance(this->position, atPosition), 2);
}

glm::mat4 Engine::PointLight::GetProjection() const
{
	return this->projection;
}

std::array<glm::mat4, 6> Engine::PointLight::GetView() const
{
	return this->views;
}

void Engine::PointLight::UpdateLight()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::POINT_LIGHTS)) >= 0)
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

		// find ourselves in the lights stack and replace the old data
		for (size_t i = 0; i < this->lights.size(); i++)
			if (this->lights.at(i) == this) {
				glNamedBufferSubData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::POINT_LIGHTS),
					i * sizeof(PointLightData), sizeof(PointLightData), &pointLight);
				break;
			}
	}
}

void Engine::PointLight::UpdateLights()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::POINT_LIGHTS)) >= 0)
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
		glNamedBufferData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::POINT_LIGHTS), (pointLights.size()) * sizeof(PointLightData), &pointLights, GL_DYNAMIC_DRAW);
	}
}

void Engine::PointLight::SetProjection()
{
	this->projection = glm::perspective(90.0f, 1.0f, 0.1f, this->GetEffectiveRadius());
}

void Engine::PointLight::SetView()
{
	// create 6 views for the cubemap shadow mapping
	this->views[0] = glm::lookAt(this->position, this->position + glm::vec3{ 1.0f, 0.0f, 0.0f },  { 0.0f, 1.0f, 0.0f });
	this->views[1] = glm::lookAt(this->position, this->position + glm::vec3{ -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	this->views[2] = glm::lookAt(this->position, this->position + glm::vec3{ 0.0f, 1.0f, 0.0f },  { 0.0f, 1.0f, 0.0f });
	this->views[3] = glm::lookAt(this->position, this->position + glm::vec3{ 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	this->views[4] = glm::lookAt(this->position, this->position + glm::vec3{ 0.0f, 0.0f, 1.0f },  { 0.0f, 1.0f, 0.0f });
	this->views[5] = glm::lookAt(this->position, this->position + glm::vec3{ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f });
}

// ============================================================================
// ---------------------------- Spot Light ------------------------------------
// ============================================================================

std::vector<Engine::SpotLight*> Engine::SpotLight::lights = {};

Engine::SpotLight::SpotLight()
{
	this->position = { 0.0f, 0.0f, 0.0f };
	this->rotation = { 0.0f, 0.0f, 0.0f };
	this->color = { 1.0f, 1.0f, 1.0f };
	this->intensity = {};
	this->angle = {};
	this->effectiveRadius = {};
	this->SetProjection();

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

float Engine::SpotLight::GetAngle() const
{
	return this->angle;
}

float Engine::SpotLight::GetSharpness() const
{
	return this->sharpness;
}

glm::mat4 Engine::SpotLight::GetProjection() const
{
	return this->projection;
}

glm::mat4 Engine::SpotLight::GetView() const
{
	return this->view;
}

float Engine::SpotLight::GetIntensityAt(glm::vec3 atPosition) const
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
	if (int(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::SPOT_LIGHTS)) >= 0)
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
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::SPOT_LIGHTS));

		// find ourselves in the lights stack and replace the old data
		#pragma omp simd
		for (size_t i = 0; i < this->lights.size(); i++)
			if (this->lights.at(i) == this) {
				glNamedBufferSubData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::SPOT_LIGHTS),
					i * sizeof(SpotLightData), sizeof(SpotLightData), &spotLight);
				break;
			}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

void Engine::SpotLight::UpdateLights()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::SPOT_LIGHTS)) >= 0)
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
		glNamedBufferData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::SPOT_LIGHTS),
			this->lights.size() * sizeof(SpotLightData), &spotLights[0], GL_DYNAMIC_DRAW);
	}
}

void Engine::SpotLight::SetAngle(float angle)
{
	this->angle = angle;

	// update the projection matrix since it depends on the angle
	this->SetProjection();

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::SpotLight::SetSharpness(float sharpness)
{
	this->sharpness = sharpness;

	// update the light stack on the GPU
	this->UpdateLight();
}

void Engine::SpotLight::SetProjection()
{
	this->projection = glm::perspective(this->angle, 1.0f, 0.1f, this->GetEffectiveRadius());
}

void Engine::SpotLight::SetView()
{
	// we're way too cool to construct a view matrix using LookAt(),
	// convert our orientation quaternion into a rotation matix
	glm::mat4 direction = glm::mat4_cast(this->orientation);

	// construct a translation matrix from our position
	glm::mat4 position = glm::mat4(1.0f);
	position = glm::translate(position, -this->position);

	// multiply the two to create a view matrix B-)
	this->view = direction * position;
}

// ============================================================================
// --------------------------- Directional Light ------------------------------
// ============================================================================

std::vector<Engine::DirectionalLight*> Engine::DirectionalLight::lights = {};

Engine::DirectionalLight::DirectionalLight()
{
	this->orientation = {0.0f, 1.0f, 0.0f};
	this->color = {1.0f, 1.0f, 1.0f};
	this->intensity = {};
	this->SetProjection();

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

glm::vec3 Engine::DirectionalLight::GetPosition() const
{
	return this->position;
}

glm::vec3 Engine::DirectionalLight::GetOrientation() const
{
	return this->orientation;
}

glm::mat4 Engine::DirectionalLight::GetProjection() const
{
	return this->projection;
}

glm::mat4 Engine::DirectionalLight::GetView() const
{
	return this->view;
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
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS));

	// find ourselves in the lights stack and replace the old data
	#pragma omp simd
	for (size_t i = 0; i < this->lights.size(); i++)
		if (this->lights.at(i) == this) {
			glNamedBufferSubData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS),
				i * sizeof(DirectionalLightData), sizeof(DirectionalLightData), &directionalLight);
			break;
		}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Engine::DirectionalLight::UpdateLights()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS)) >= 0)
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
		glNamedBufferData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS),
			(this->lights.size()) * sizeof(DirectionalLightData), &directionalLights[0], GL_DYNAMIC_DRAW);
	}
}

void Engine::DirectionalLight::SetProjection()
{
	// construct an ortographic projection for the directional light
							// left, right
	this->projection = glm::ortho(-this->projectionSize / 2.0f, this->projectionSize / 2.0f,
							// bottom, top
							-this->projectionSize / 2.0f, this->projectionSize / 2.0f,
							// near, far
							0.1f, 1000.0f);
}

void Engine::DirectionalLight::SetView()
{
	this->view = glm::lookAt(this->position, this->position + this->orientation, { 0.0f, 1.0f, 0.0f });
}

// ============================================================================
// ---------------------------- Ambient Light ---------------------------------
// ============================================================================

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
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::AMBIENT_LIGHTS));

	// find ourselves in the lights stack and replace the old data
	#pragma omp simd
	for (size_t i = 0; i < this->lights.size(); i++)
		if (this->lights.at(i) == this) {
			glNamedBufferSubData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::AMBIENT_LIGHTS),
				i * sizeof(AmbientLightData), sizeof(AmbientLightData), &ambientLight);
			break;
		}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Engine::AmbientLight::UpdateLights()
{
	// check if a point light stack actually exists on the GPU
	if (int(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::AMBIENT_LIGHTS)) >= 0)
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
		glNamedBufferData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::AMBIENT_LIGHTS),
			this->lights.size() * sizeof(AmbientLightData), &ambientLights[0], GL_DYNAMIC_DRAW);
	}
}