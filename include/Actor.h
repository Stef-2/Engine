#ifndef ACTOR_H
#define ACTOR_H

#include "Object.h"
#include "Camera.h"

namespace Engine
{

	//forward declaration of Engine::Camera
	//so it can be referenced by the draw function without causing a cyclic dependancy
	class Camera;

	//class encapsulating objects meant to be rendered
	//and collided with each other
	class Actor : public Engine::Object
	{
		using Engine::Object::Object;

	public:
		Engine::Shader* GetShader();
		Engine::Model* GetModel();

		void MoveRelative(glm::vec3 direction, float intensity) override;
		void MoveRelative(float x, float y, float z) override;
		void MoveAbsolute(float x, float y, float z) override;

		void SetShader(const Engine::Shader& shader);
		void SetModel(const Engine::Model& model);

		//draw using custom view and projection matrices instead of a camera
		void Draw(glm::mat4 view, glm::mat4 projection);
		//draw using a camera which provides the view and projection matrices
		void Draw(Engine::Camera* camera);

	protected:
		Model model;
		Shader shader;
	};

}
#endif