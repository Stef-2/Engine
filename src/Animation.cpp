#include "Animation.h"

Engine::Animation::Animation()
{
    this->duration = {};
    this->ticksPerSecond = {};
    this->name = {};
    this->nodes = {};
}

Engine::Animation::Animation(std::string name, float duration, float ticksPerSecond, std::vector<Engine::AnimationNode> nodes)
{
    this->duration = duration;
    this->ticksPerSecond = ticksPerSecond;
    this->name = name;
    this->nodes = nodes;

    this->Setup();
}

std::vector<Engine::AnimationNode>& Engine::Animation::GetNodes()
{
    return this->nodes;
}

std::string Engine::Animation::GetName()
{
    return this->name;
}

float Engine::Animation::GetDuration()
{
    return this->duration;
}

float Engine::Animation::GetTicksPerSecond()
{
    return this->ticksPerSecond;
}

void Engine::Animation::SetNodes(std::vector<Engine::AnimationNode>& nodes)
{
    this->nodes = nodes;
}

void Engine::Animation::SetName(std::string& name)
{
    this->name = name;
}

void Engine::Animation::SetDuration(float duration)
{
    this->duration = duration;
}

void Engine::Animation::SetTicksPerSecond(float ticksPerSecond)
{
    this->ticksPerSecond = ticksPerSecond;
}

void Engine::Animation::Setup()
{

}

