#ifndef LIGHT_H
#define LIGHT_H

#include "Object.h"
#include "Mesh.h"
#include "Shader.h"

#include "glm/glm.hpp"

#include "vector"

namespace Engine
{
	// abstract base light class, defining common light attributes
	// it is to be inherited by more specialized lights
	class Light
	{
	public:
		virtual void* GetData() = 0;

		glm::vec3 GetColor();
		float GetIntensity();

		// calculates this lights intensity at a given position
		virtual float GetIntensity(glm::vec3 atPosition) = 0;

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
		float GetIntensity(glm::vec3 atPosition) override;
		Engine::Mesh& GetMesh();
		Engine::Shader& GetShader();

		void SetMesh(Engine::Mesh mesh);
		void SetShader(Engine::Shader shader);

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

		void* GetData() override;

		struct Data
		{
			Data(Engine::PointLight&);

			glm::vec3& position;
			glm::vec3& color;
			float& intensity;
		};

		static std::vector<Engine::PointLight::Data*>& GetLights();

	private:
		Data data;

		static std::vector<Engine::PointLight::Data*> lights;
	};
	
	// specialized physical light
	// has a position, orientation, mesh and shader
	// radiates light in a cone
	class SpotLight : public Engine::PhysicalLight
	{
	public:
		SpotLight();

		struct Data
		{
			Data(Engine::SpotLight&);

			glm::vec3& position;
			glm::vec3& rotation;
			glm::vec3& color;
			float& intensity;
			float angle;
		};

		static std::vector<Engine::SpotLight::Data*>& GetLights();

		void* GetData() override;

	private:
		Data data;

		static std::vector<Engine::SpotLight::Data*> lights;
	};

	// specialized non physical light
	// has an orientation
	// radiates parallel light rays with an infinite radius and no decay
	class DirectionalLight : public Engine::Light
	{
	public:
		DirectionalLight();

		struct Data
		{
			Data(Engine::DirectionalLight&);

			glm::vec3& rotation;
			glm::vec3& color;
			float& intensity;
		};

		static std::vector<Engine::DirectionalLight::Data*>& GetLights();
		glm::vec3 GetRotation();

		void SetRotation(glm::vec3 rotation);
		void* GetData() override;

	private:
		 Data data;

		static std::vector<Engine::DirectionalLight::Data*> lights;
		glm::vec3 rotation;
	};

	// specialized non physical light
	// has a position
	// applies linearly decaying lighting to all meshes in affected radius
	class AmbientLight : public Engine::Light
	{
	public:
		AmbientLight();

		struct Data
		{
			Data(Engine::AmbientLight&);

			glm::vec3& position;
			glm::vec3& color;
			float& intensity;
		};

		static std::vector<Engine::AmbientLight::Data*>& GetLights();
		glm::vec3 GetPosition();

		void SetPosition(glm::vec3 position);
		void* GetData() override;

	private:
		 Data data;

		static std::vector<Engine::AmbientLight::Data*> lights;
		glm::vec3 position;
	};
	
}

#endif
