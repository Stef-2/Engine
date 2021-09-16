#ifndef LIGHT_H
#define LIGHT_H

#include "Object.h"
#include "Mesh.h"
#include "Shader.h"

#include "glm/glm.hpp"

namespace Engine
{
	// abstract base light class, defining common light attributes
	// it is to be inherited by more specialized lights
	class Light
	{
	public:
		virtual void* GetData() = 0;

		glm::vec3 GetColor();
		double GetIntensity();

		// calculates this lights intensity at a given position
		virtual double GetIntensity(glm::vec3 atPosition) = 0;

		void SetIntensity(double intensity);
		void SetColor(glm::vec3 color);

	protected:
		double intensity;
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
		double GetIntensity(glm::vec3 atPosition) override;
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

		void* GetData() override;

	private:
		struct Data
		{
			Data(Engine::PointLight&);

			glm::vec3& position;
			glm::vec3& color;
			double& intensity;
		} data;

	};
	
	// specialized physical light
	// has a position, orientation, mesh and shader
	// radiates light in a cone
	class SpotLight : public Engine::PhysicalLight
	{
	public:
		SpotLight();

		void* GetData() override;

	private:
		struct Data
		{
			Data(Engine::SpotLight&);

			glm::vec3& position;
			glm::vec3& color;
			double& intensity;
			double angle;
		} data;

	};

	// specialized non physical light
	// has an orientation
	// radiates parallel light rays with an infinite radius and no decay
	class DirectionalLight : public Engine::Light
	{
	public:
		DirectionalLight();

		glm::vec3 GetOrientation();

		void SetOrientation(glm::vec3 orientation);
		void* GetData() override;

	private:
		struct Data
		{
			Data(Engine::DirectionalLight&);

			glm::vec3& orientation;
			glm::vec3& color;
			double& intensity;
		} data;

		glm::vec3 orientation;
	};

	// specialized non physical light
	// has a position
	// applies linearly decaying lighting to all meshes in affected radius
	class AmbientLight : public Engine::Light
	{
	public:
		AmbientLight();

		glm::vec3 GetPosition();

		void SetPosition(glm::vec3 position);
		void* GetData() override;

	private:
		struct Data
		{
			Data(Engine::AmbientLight&);

			glm::vec3& position;
			glm::vec3& color;
			double& intensity;
		} data;

		glm::vec3 position;
	};
	
}
#endif
