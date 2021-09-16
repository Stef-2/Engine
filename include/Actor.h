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
		inline Engine::Shader& GetShader();
		inline Engine::Model& GetModel();

		void MoveRelative(glm::vec3 direction, float magnitude) override;
		void MoveRelative(float x, float y, float z) override;
		void MoveAbsolute(float x, float y, float z) override;

		inline void SetShader(const Engine::Shader& shader);
		inline void SetModel(const Engine::Model& model);

	private:
		Model model;
		Shader shader;

		bool isVisible;
	};

}
#endif