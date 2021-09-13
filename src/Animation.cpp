#include "Animation.h"

Engine::Animation::Animation()
{
    this->duration = {};
    this->ticksPerSecond = {};
    this->name = {};
    this->startTime = 0.0;
    this->nodeAnimations = {};
}

Engine::Animation::Animation(std::string name, double duration, double ticksPerSecond)
{
    this->duration = duration;
    this->ticksPerSecond = ticksPerSecond;
    this->name = name;
    this->startTime = 0.0;
    this->nodeAnimations = {};

    this->Setup();
}

Engine::Animation::Animation(std::string name, double duration, double ticksPerSecond, std::vector<Engine::NodeAnimation> nodeAnimations)
{
    this->duration = duration;
    this->ticksPerSecond = ticksPerSecond;
    this->name = name;
    this->nodeAnimations = nodeAnimations;
    this->startTime = 0.0;

    this->Setup();
}

std::string Engine::Animation::GetName()
{
    return this->name;
}

double Engine::Animation::GetDuration()
{
    return this->duration;
}

double Engine::Animation::GetTicksPerSecond()
{
    return this->ticksPerSecond;
}

double Engine::Animation::GetStartTime()
{
    return this->startTime;
}

std::vector<Engine::NodeAnimation>& Engine::Animation::GetNodeAnimations()
{
    return this->nodeAnimations;
}

void Engine::Animation::SetName(std::string& name)
{
    this->name = name;
}

void Engine::Animation::SetDuration(double duration)
{
    this->duration = duration;
}

void Engine::Animation::SetTicksPerSecond(double ticksPerSecond)
{
    this->ticksPerSecond = ticksPerSecond;
}

void Engine::Animation::SetNodeAnimations(std::vector<Engine::NodeAnimation> nodeAnimations)
{
    this->nodeAnimations = nodeAnimations;
}
void Engine::Animation::AddNodeAnimation(Engine::NodeAnimation nodeAnimation)
{
    this->nodeAnimations.push_back(nodeAnimation);
}

void Engine::Animation::SetStartTime(double startTime)
{
    this->startTime = startTime;
}

void Engine::Animation::Setup()
{

}

