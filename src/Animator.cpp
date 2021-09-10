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
		if (animation->GetStartTime() == -1.0) {
			animation->SetStartTime(currentTime);
			//std::cout << "animation hasnt started yet" << std::endl;
			this->UpdateAnimation(actor, *animation);
		}
		else
			// check if we're done
			if (animation->GetStartTime() + animation->GetDuration() >= currentTime) {
				animation->SetStartTime(0.0);
				this->UpdateAnimation(actor, *animation);
				//std::cout << "animation is done" << std::endl;
			}
			else {
				// else, animate
				this->UpdateAnimation(actor, *animation);
				//std::cout << "animating" << std::endl;
			}
	}
	else
		std::cerr << "Could not find an animation with the given name: " << animationName << std::endl;
}

void Engine::Animator::UpdateAnimation(Engine::Actor& actor, Engine::Animation animation)
{
	double currentTime = glfwGetTime();
	double timeOffset = currentTime - animation.GetStartTime();
	//std::cout << "animation total time: " << animation.GetDuration() << std::endl;
	//timeOffset = timeOffset < 0.1 ? 0.1 : timeOffset;
	//std::cout << "animation time: " << timeOffset << std::endl;
	//std::cout << "animating" << std::endl;
	for (size_t i = 0; i < actor.GetModel().GetAnimatedMeshes().size(); i++)
		for (size_t j = 0; j < actor.GetModel().GetAnimatedMeshes().at(i).GetSkeleton().GetBones().size(); j++)
			actor.GetModel().GetAnimatedMeshes().at(i).GetSkeleton().GetBones().at(j).SetAnimationTime(timeOffset);
}

float Engine::Animator::GetSpeedMultiplier()
{
	return this->speedMultiplier;
}

void Engine::Animator::SetSpeedMultiplier(float multiplier)
{
	this->speedMultiplier = multiplier;
}
