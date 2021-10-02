#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Actor.h"

#include "glm/glm.hpp"
#include "glfw3.h"

#include "vector"

namespace Engine
{
	// utility struct that maps an actor to one or more of its animations
	struct AnimationStack
	{
		Engine::Actor* actor;
		std::vector<Engine::Animation*> animations;
	};

	class Animator
	{
	public:
		Animator();
		Animator(float speedMultiplier);

		// start an animation
		void Animate(Engine::Actor& actor, std::string animationName);
		// update all running animations
		void UpdateAnimations();

		// set the actor into its T-Pose, basically cybeprunk 2077 mode
		void TPose();

		float GetSpeedMultiplier();

		void SetSpeedMultiplier(float multiplier);

	private:
		// stack of currently running animations
		std::vector<Engine::AnimationStack> runningAnimations;

		float speedMultiplier;
	};
}
#endif // ANIMATOR_H