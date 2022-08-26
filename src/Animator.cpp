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
	Engine::ActorAnimationStack animationStack;

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
		for (size_t i = 0; i < this->runningActorAnimations.size(); i++)

			// check if the actor already exist in the stack
			if (this->runningActorAnimations.at(i).actor == &actor) {

				// check if the animation already exists
				for (size_t j = 0; j < this->runningActorAnimations.at(i).animations.size(); j++)
					if (this->runningActorAnimations.at(i).animations.at(j) == animation)
						goto exit;

				// if not, just add the new animation
				this->runningActorAnimations.at(i).animations.push_back(animation);
				goto exit;
			}

		// if the given actor wasn't found, create a new animation stack for it and push it back
		animationStack.actor = &actor;
		animationStack.animations.push_back(animation);
		this->runningActorAnimations.push_back(animationStack);

	exit:;
		
	}
	else
		std::cerr << "Could not find an animation with the given name: " << animationName << std::endl;
}

void Engine::Animator::Animate(Engine::UserInterface::UIElement& element, UIElementAnimation& animation)
{
	double currentTime = glfwGetTime();

	Engine::UIAnimationStack newAnimation{&element, &animation, element.GetPosition(), currentTime + animation.time};

	this->runningUIAnimations.emplace_back(newAnimation);
}

void Engine::Animator::UpdateAnimations()
{
	double currentTime = glfwGetTime();

	// go through all running animation stacks
	for (size_t i = 0; i < this->runningActorAnimations.size(); i++)
	{
		// go through all the animations within
		for (size_t j = 0; j < this->runningActorAnimations.at(i).animations.size(); j++)
		{
			Engine::Animation& animation = *this->runningActorAnimations.at(i).animations.at(j);

			// check if the animation is done
			if (animation.GetStartTime() + animation.GetDuration() < currentTime) {
				// remove it from the stack
				this->runningActorAnimations.at(i).animations.erase(this->runningActorAnimations.at(i).animations.begin() + j);
				this->runningActorAnimations.at(i).animations.shrink_to_fit();
			}

			// check if the actor has no more active animations after removal of last one
			if (!this->runningActorAnimations.at(i).animations.size()) {
				// remove the entire animation stack
				this->runningActorAnimations.erase(this->runningActorAnimations.begin() + i);
				this->runningActorAnimations.shrink_to_fit();
				return;
			}
		}
	}

	// check if there are any animation stacks left after the pruning
	if (this->runningActorAnimations.size() > 0) {
		// animate the remaining animations
		for (size_t i = 0; i < this->runningActorAnimations.size(); i++)
		{
			// go through all the animations within
			for (size_t j = 0; j < this->runningActorAnimations.at(i).animations.size(); j++)
			{
				Engine::Actor& actor = *this->runningActorAnimations.at(i).actor;
				Engine::Animation& animation = *this->runningActorAnimations.at(i).animations.at(j);

				double timeOffset = currentTime - animation.GetStartTime();

				animation.TransformNodes(timeOffset);
			}
		}
	}

	// animate UI elements
	for (auto& animation : this->runningUIAnimations)
	{
		// check if we're in the time range
		if (currentTime < animation.targetTime)
		{
			// based on current time, calculate where in the animation are we
			float startTime = animation.targetTime - animation.animation->time;
			float middle = currentTime - startTime;
			float delta = animation.targetTime - startTime;
			float scale = middle / delta;
			glm::vec2 move = glm::mix(animation.originalPosition, animation.originalPosition + animation.animation->translation, scale);
			animation.element->MoveAbsolute(move.x, move.y);
		}
	}

	// remove any UI animations past their expiry date
	this->runningUIAnimations.erase(std::remove_if
	(runningUIAnimations.begin(), runningUIAnimations.end(),
		[&currentTime](UIAnimationStack& e) {return currentTime > e.targetTime;}),
		runningUIAnimations.end());
}

float Engine::Animator::GetSpeedMultiplier()
{
	return this->speedMultiplier;
}

void Engine::Animator::SetSpeedMultiplier(float multiplier)
{
	this->speedMultiplier = multiplier;
}
