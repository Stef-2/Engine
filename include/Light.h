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
		virtual ~PhysicalLight() = 0;

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

	private:


	};
	
	// specialized physical light
	// has a position, orientation, mesh and shader
	// radiates light in a cone
	class SpotLight : public Engine::PhysicalLight
	{
	public:

	private:


	};

	// specialized non physical light
	// has an orientation
	// radiates parallel light rays with an infinite radius and no decay
	class DirectionalLight : public Engine::Light
	{
	public:

	private:
	};

	// specialized non physical light
	// has a position
	// applies linearly decaying lighting to all meshes in affected radius
	class AmbientLight : public Engine::Light
	{
	public:

	private:
	};
	
}
#endif
