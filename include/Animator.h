#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Actor.h"
#include "UserInterface.h"

#include "glm/glm.hpp"
#include "glfw3.h"

#include "vector"

namespace Engine
{
	// utility struct that maps an actor to one or more of its animations
	struct ActorAnimationStack
	{
		Engine::Actor* actor;
		std::vector<Engine::Animation*> animations;
	};

	// utility struct that maps UIelements and their animations
	struct UIAnimationStack
	{
		Engine::UserInterface::UIElement* element;
		Engine::UIElementAnimation* animation;

		glm::vec2 originalPosition;

		double targetTime;
	};

	// animation coordinator
	class Animator
	{
	public:
		Animator();
		Animator(float speedMultiplier);

		// start an actor animation
		void Animate(Engine::Actor& actor, std::string animationName);
		void Animate(Engine::UserInterface::UIElement& element, UIElementAnimation& animation);

		// update all running animations
		void UpdateAnimations();

		// set the actor into its T-Pose, basically cybeprunk 2077 mode
		void TPose();

		float GetSpeedMultiplier();

		void SetSpeedMultiplier(float multiplier);

	private:
		// stack of currently running animations
		std::vector<Engine::ActorAnimationStack> runningActorAnimations;
		std::vector<Engine::UIAnimationStack> runningUIAnimations;

		float speedMultiplier;
	};
}
#endif // ANIMATOR_H