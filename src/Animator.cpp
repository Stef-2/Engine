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
	Engine::AnimationStack animationStack;

	// find the requested animation
	for (size_t i = 0; i < actor.GetModel().GetAnimatedMeshes().size(); i++)
		for (size_t j = 0; j < actor.GetModel().GetAnimatedMeshes().at(i)->GetAnimations().size(); j++)
			if (actor.GetModel().GetAnimatedMeshes().at(i).get()->GetAnimations().at(j).GetName() == animationName)
				animation = &actor.GetModel().GetAnimatedMeshes().at(i).get()->GetAnimations().at(j);
	
	// check if we found the animation with the given name
	if (animation)
	{
		// set the start parameters
		animation->SetStartTime(currentTime);
		animation->SetCurrentPositionKey(0u);
		animation->SetCurrentRotationKey(0u);
		animation->SetCurrentScaleKey(0u);

		// create or update a mapping between an actor and its animation(s)
		for (size_t i = 0; i < this->runningAnimations.size(); i++)

			// check if the actor already exist in the stack
			if (this->runningAnimations.at(i).actor == &actor) {

				// check if the animation already exists
				for (size_t j = 0; j < this->runningAnimations.at(i).animations.size(); j++)
					if (this->runningAnimations.at(i).animations.at(j) == animation)
						goto exit;

				// if not, just add the new animation
				this->runningAnimations.at(i).animations.push_back(animation);
				goto exit;
			}

		// if the given actor wasn't found, create a new animation stack for it and push it back
		animationStack.actor = &actor;
		animationStack.animations.push_back(animation);
		this->runningAnimations.push_back(animationStack);

	exit:;
		
	}
	else
		std::cerr << "Could not find an animation with the given name: " << animationName << std::endl;
}

void Engine::Animator::UpdateAnimations()
{
	double currentTime = glfwGetTime();

	// go through all running animation stacks
	for (size_t i = 0; i < this->runningAnimations.size(); i++)
	{
		// go through all the animations within
		for (size_t j = 0; j < this->runningAnimations.at(i).animations.size(); j++)
		{
			Engine::Animation& animation = *this->runningAnimations.at(i).animations.at(j);

			// check if the animation is done
			if (animation.GetStartTime() + animation.GetDuration() < currentTime) {
				// remove it from the stack
				this->runningAnimations.at(i).animations.erase(this->runningAnimations.at(i).animations.begin() + j);
				this->runningAnimations.at(i).animations.shrink_to_fit();
			}

			// check if the actor has no more active animations after removal of last one
			if (!this->runningAnimations.at(i).animations.size()) {
				// remove the entire animation stack
				this->runningAnimations.erase(this->runningAnimations.begin() + i);
				this->runningAnimations.shrink_to_fit();
				return;
			}
		}
	}

	// check if there are any animation stacks left after the pruning
	if (this->runningAnimations.size() > 0) {
		// animate the remaining animations
		for (size_t i = 0; i < this->runningAnimations.size(); i++)
		{
			// go through all the animations within
			for (size_t j = 0; j < this->runningAnimations.at(i).animations.size(); j++)
			{
				Engine::Actor& actor = *this->runningAnimations.at(i).actor;
				Engine::Animation& animation = *this->runningAnimations.at(i).animations.at(j);

				double timeOffset = currentTime - animation.GetStartTime();

				animation.TransformNodes(timeOffset);
			}
		}
	}
}

float Engine::Animator::GetSpeedMultiplier()
{
	return this->speedMultiplier;
}

void Engine::Animator::SetSpeedMultiplier(float multiplier)
{
	this->speedMultiplier = multiplier;
}
