#ifndef LIGHT_H
#define LIGHT_H

#include "Object.h"
#include "Mesh.h"
#include "Shader.h"

namespace Engine
{
	// base light class, defining common light attributes
	// it inherits transformation handling mechanisms from Engine::Object
	// it is to be inherited by more specialized lights
	class Light : public Engine::Object
	{
		using Engine::Object::Object;

	public:

		glm::vec3 GetColor();
		double GetIntensity();

		void SetIntensity(double intensity);
		void SetColor(glm::vec3 color);

	protected:
		Engine::Mesh mesh;
		Engine::Shader shader;

		double intensity;
		glm::vec3 color;
	};

	// base phyisical light class
	// acting as a base for lights with positions and orientations
	// they are meant to be renderable and thus have meshes and shaders
	class PhysicalLight : public Engine::Light
	{
	public:
		Engine::Mesh& GetMesh();
		Engine::Shader& GetShader();

		void SetMesh(Engine::Mesh mesh);
		void SetShader(Engine::Shader shader);

	private:
		Engine::Mesh mesh;
		Engine::Shader shader;
	};

	class PointLight : public Engine::PhysicalLight
	{
	public:

	private:

	};

	class SpotLight : public Engine::PhysicalLight
	{
	public:

	private:

	};
	
}
#endif
