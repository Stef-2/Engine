#include "Animation.h"

Engine::Animation::Animation()
{
    this->duration = {};
    this->ticksPerSecond = {};
    this->name = {};
    //this->positionKeyFrames = {};
    //this->rotationKeyFrames = {};
    //this->scaleKeyFrames = {};
    this->startTime = {};
}

/*Engine::Animation::Animation(std::string name, double duration, double ticksPerSecond,
                             std::vector<Engine::VectorKeyFrame> positionKeyFrames,
                             std::vector<Engine::QuaternionKeyFrame> rotationKeyFrames,
                             std::vector<Engine::VectorKeyFrame> scaleKeyFrames)
{
    this->duration = duration;
    this->ticksPerSecond = ticksPerSecond;
    this->name = name;
    this->positionKeyFrames = positionKeyFrames;
    this->rotationKeyFrames = rotationKeyFrames;
    this->scaleKeyFrames = scaleKeyFrames;
    this->startTime = {};

    this->Setup();
}

std::vector<Engine::VectorKeyFrame>& Engine::Animation::GetPositionKeyFrames()
{
    return this->positionKeyFrames;
}

std::vector<Engine::QuaternionKeyFrame>& Engine::Animation::GetRotationKeyFrames()
{
    return this->rotationKeyFrames;
}

std::vector<Engine::VectorKeyFrame>& Engine::Animation::GetScaleKeyFrames()
{
    return this->scaleKeyFrames;
}
*/

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
/*
void Engine::Animation::SetPositionKeyFrames(std::vector<Engine::VectorKeyFrame> keyFrames)
{
    this->positionKeyFrames = keyFrames;
}

void Engine::Animation::SetRotationKeyFrames(std::vector<Engine::QuaternionKeyFrame> keyFrames)
{
    this->rotationKeyFrames = keyFrames;
}

void Engine::Animation::SetScaleKeyFrames(std::vector<Engine::VectorKeyFrame> keyFrames)
{
    this->scaleKeyFrames = keyFrames;
}
*/
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

void Engine::Animation::SetStartTime(double startTime)
{
    this->startTime = startTime;
}

void Engine::Animation::Setup()
{

}

