#ifndef ACTOR_H
#define ACTOR_H

#include "Object.h"

namespace Engine
{

	//class encapsulating objects meant to be rendered
	//and collided with each other
	class Actor : public Engine::Object
	{
		using Engine::Object::Object;

	public:
		Engine::Shader* GetShader();
		Engine::Model* GetModel();

		void SetShader(const Engine::Shader& shader);
		void SetModel(const Engine::Model& model);

		void Draw(glm::mat4 view, glm::mat4 projection);

	protected:
		Model model;
		Shader shader;
	};
}
#endif