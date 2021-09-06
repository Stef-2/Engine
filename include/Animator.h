#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animation.h"

#include "glm/glm.hpp"
#include "glfw3.h"

namespace Engine
{

	class Animator
	{
	public:
		Animator();
		Animator(float speedMultiplier);

		void Animate(Engine::Animation& animation);

		float GetSpeedMultiplier();

		void SetSpeedMultiplier(float multiplier);

	private:
		void UpdateAnimation(Engine::Animation& animation);

		float speedMultiplier;
	};
}
#endif // ANIMATOR_H