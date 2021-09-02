#ifndef ACTOR_H
#define ACTOR_H

#include "Object.h"

namespace Engine
{

	// class encapsulating objects meant to be rendered and collided with each other
	class Actor : public Engine::Object
	{
		using Engine::Object::Object;

	public:
		Engine::Shader* GetShader();
		Engine::Model* GetModel();

		void MoveRelative(glm::vec3 direction, float magnitude) override;
		void MoveRelative(float x, float y, float z) override;
		void MoveAbsolute(float x, float y, float z) override;

		void SetShader(const Engine::Shader& shader);
		void SetModel(const Engine::Model& model);

	protected:
		Model model;
		Shader shader;

		bool isVisible;
	};

}
#endif