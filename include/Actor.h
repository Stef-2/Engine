#ifndef ACTOR_H
#define ACTOR_H

#include "Object.h"
#include "Shared.h"

namespace Engine
{

	// class encapsulating objects meant to be rendered and collided with each other
	class Actor : public Engine::Object
	{
		using Engine::Object::Object;

	public:
		Engine::ShaderProgram& GetShader();
		Engine::Model& GetModel();

		void MoveRelative(glm::vec3 direction, float magnitude) override;
		void MoveRelative(float x, float y, float z) override;
		void MoveAbsolute(float x, float y, float z) override;

		void SetShader(const Engine::ShaderProgram& shader);
		void SetShader(const Engine::ShaderProgram* shader);
		void SetShader(const std::shared_ptr<Engine::ShaderProgram>& shader);

		void SetModel(const Engine::Model& model);
		void SetModel(const Engine::Model* model);
		void SetModel(const std::shared_ptr<Engine::Model>& model);

	private:
		Engine::Shared<Engine::Model> model;
		std::shared_ptr<Engine::ShaderProgram> shader;

		bool isVisible;
	};

}
#endif