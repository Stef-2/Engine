#include "Animator.h"

Engine::Animator::Animator()
{
	this->speedMultiplier = {};
}

Engine::Animator::Animator(float speedMultiplier)
{
	this->speedMultiplier = speedMultiplier;
}

void Engine::Animator::Animate(Engine::Actor& actor, std::string animationName)
{
	double currentTime = glfwGetTime();
	Engine::Animation* animation = nullptr;

	// find the requested animation
	for (size_t i = 0; i < actor.GetModel().GetAnimations().size(); i++)
		if (actor.GetModel().GetAnimations().at(i).GetName() == animationName)
			animation = &actor.GetModel().GetAnimations().at(i);
	
	// check if we found the animation with the given name
	if (animation)
	{
		// check if we haven't started yet
		if (!animation->GetStartTime()) {
			animation->SetStartTime(currentTime);
			this->UpdateAnimation(actor, *animation);
		}
		else
			// check if we're done
			if (animation->GetStartTime() + animation->GetDuration() >= currentTime)
				animation->SetStartTime(0.0);
			else
				// else, animate
				this->UpdateAnimation(actor, *animation);
	}
	else
		std::cerr << "Could not find an animation with the given name: " << animationName << std::endl;
}

void Engine::Animator::UpdateAnimation(Engine::Actor& actor, Engine::Animation)
{
	double currentTime = glfwGetTime();
}

float Engine::Animator::GetSpeedMultiplier()
{
	return this->speedMultiplier;
}

void Engine::Animator::SetSpeedMultiplier(float multiplier)
{
	this->speedMultiplier = multiplier;
}
