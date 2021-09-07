#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Actor.h"

#include "glm/glm.hpp"
#include "glfw3.h"

namespace Engine
{

	class Animator
	{
	public:
		Animator();
		Animator(float speedMultiplier);

		void Animate(Engine::Actor& actor, std::string animationName);

		float GetSpeedMultiplier();

		void SetSpeedMultiplier(float multiplier);

	private:
		void UpdateAnimation(Engine::Actor& actor, Engine::Animation);

		float speedMultiplier;
	};
}
#endif // ANIMATOR_H