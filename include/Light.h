#ifndef LIGHT_H
#define LIGHT_H

#include "Object.h"
#include "Mesh.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

#include "vector"

namespace Engine
{
	// abstract base light class, defining common light attributes
	// it is to be inherited by more specialized lights
	class Light
	{
	public:
		glm::vec3 GetColor();
		float GetIntensity();

		virtual void UpdateLight() = 0;
		virtual void UpdateLights() = 0;

		void SetIntensity(float intensity);
		void SetColor(glm::vec3 color);

	protected:
		float intensity;
		glm::vec3 color;
	};
	
	// abstract base physical light class
	// it inherits transformation handling mechanisms from Engine::Object and common light properties from Engine::Light
	// acting as a base for lights with physical positions, orientations and an inverse square intensity decay
	// they are meant to be renderable and thus have meshes and shaders
	class PhysicalLight : public Engine::Light, public Engine::Object
	{
		using Engine::Object::Object;

	public:
		// calculates this lights intensity at a given position
		virtual float GetIntensityAt(glm::vec3 atPosition) = 0;

		Engine::Mesh& GetMesh();
		Engine::Shader& GetShader();

		void SetMesh(Engine::Mesh mesh);
		void SetShader(Engine::Shader shader);

		void MoveRelative(glm::vec3 direction, float intensity) override;
		void MoveRelative(float x, float y, float z) override;
		void MoveAbsolute(float x, float y, float z) override;

		virtual void RotateRelative(float x, float y, float z) override;
		virtual void RotateAbsolute(float x, float y, float z) override;

	private:
		Engine::Mesh mesh;
		Engine::Shader shader;
	};
	
	// specialized physical light
	// has a position, mesh and shader
	// radiates light in all directions
	class PointLight : public Engine::PhysicalLight
	{
	public:
		PointLight();
		~PointLight();

		static std::vector<Engine::PointLight*>& GetLights();

		// calculates this lights intensity at a given position
		float GetIntensityAt(glm::vec3 atPosition) override;
		float GetEffectiveRadius();

		void UpdateLight() override;
		void UpdateLights() override;
	private:

		static std::vector<Engine::PointLight*> lights;
	};
	
	// specialized physical light
	// has a position, orientation, angle, mesh and shader
	// radiates light in a cone
	class SpotLight : public Engine::PhysicalLight
	{
	public:
		SpotLight();
		~SpotLight();

		static std::vector<Engine::SpotLight*>& GetLights();
		float GetIntensityAt(glm::vec3 atPosition) override;
		float GetAngle();
		float GetSharpness();

		void UpdateLight() override;
		void UpdateLights() override;
		void SetAngle(float angle);
		void SetSharpness(float sharpness);

	private:
		static std::vector<Engine::SpotLight*> lights;
		float angle;
		float sharpness;
	};

	// specialized non physical light
	// has an orientation and intensity
	// radiates parallel light rays with an infinite radius and no decay
	class DirectionalLight : public Engine::Light
	{
	public:
		DirectionalLight();
		~DirectionalLight();

		static std::vector<Engine::DirectionalLight*>& GetLights();
		glm::vec3 GetPosition();
		glm::vec3 GetRotation();

		void SetPosition(glm::vec3 position);
		void SetRotation(glm::vec3 rotation);
		void UpdateLight() override;
		void UpdateLights() override;

	private:
		static std::vector<Engine::DirectionalLight*> lights;
		glm::vec3 position;
		glm::vec3 rotation;
	};

	// specialized non physical light
	// has a position and intensity
	// applies linearly decaying lighting to all meshes in affected radius
	class AmbientLight : public Engine::Light
	{
	public:
		AmbientLight();
		~AmbientLight();

		static std::vector<Engine::AmbientLight*>& GetLights();
		glm::vec3 GetPosition();

		void SetPosition(glm::vec3 position);
		void UpdateLight() override;
		void UpdateLights() override;

	private:
		static std::vector<Engine::AmbientLight*> lights;
		glm::vec3 position;
	};
	
}

#endif
