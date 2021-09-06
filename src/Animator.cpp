#include "Animator.h"

Engine::Animator::Animator()
{
	this->speedMultiplier = {};
}

Engine::Animator::Animator(float speedMultiplier)
{
	this->speedMultiplier = speedMultiplier;
}

void Engine::Animator::Animate(Engine::Animation& animation)
{
	// check if we haven't started yet
	if (!animation.GetStartTime()) {
		animation.SetStartTime(glfwGetTime());
		this->UpdateAnimation(animation);
	}
	else
		// check if we're done
		if (animation.GetStartTime() + animation.GetDuration() >= glfwGetTime())
			animation.SetStartTime(0.0);
		else
			// else, animate
			this->UpdateAnimation(animation);
}

void Engine::Animator::UpdateAnimation(Engine::Animation& animation)
{
	
}

float Engine::Animator::GetSpeedMultiplier()
{
	return this->speedMultiplier;
}

void Engine::Animator::SetSpeedMultiplier(float multiplier)
{
	this->speedMultiplier = multiplier;
}
