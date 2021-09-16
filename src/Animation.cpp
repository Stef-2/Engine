#include "Animation.h"

Engine::Animation::Animation()
{
    this->duration = {};
    this->ticksPerSecond = {};
    this->name = {};
    this->startTime = 0.0;
    this->nodeAnimations = {};

    this->currentPositionKey = {};
    this->currentRotationKey = {};
    this->currentScaleKey = {};
}

Engine::Animation::Animation(std::string name, double duration, double ticksPerSecond)
{
    this->duration = duration;
    this->ticksPerSecond = ticksPerSecond;
    this->name = name;
    this->startTime = 0.0;
    this->nodeAnimations = {};

    this->currentPositionKey = {};
    this->currentRotationKey = {};
    this->currentScaleKey = {};
}

Engine::Animation::Animation(std::string name, double duration, double ticksPerSecond, std::vector<Engine::NodeAnimation> nodeAnimations)
{
    this->duration = duration;
    this->ticksPerSecond = ticksPerSecond;
    this->name = name;
    this->nodeAnimations = nodeAnimations;
    this->startTime = 0.0;

    this->currentPositionKey = {};
    this->currentRotationKey = {};
    this->currentScaleKey = {};
}

glm::mat4 Engine::Animation::TransformNode(Engine::NodeAnimation& nodeAnimation, double timeOffset)
{
    // prepare an identity matrix
    glm::mat4 transform = glm::mat4(1.0f);

    // scaling factor to be used for linear interpolation
    float scaleFactor = 0;

    //vectors to be used for final transforms
    glm::vec3 scale;
    glm::quat rotate;
    glm::vec3 translate;

    // utility lambda that finds the normalized scale between the two frames based on current time
    auto KeyFrameScale = [&timeOffset](double before, double after) -> float
    {
        float middle = timeOffset - before;
        float frameDelta = after - before;

        return middle / frameDelta;
    };

    // go through all keyframes beggining at the last one and find the ones that match the given time offset
    // transform the inherited matrix using keyframed values
    // order of transformations matters
    // the correct order is Scale, Rotate, Translate (SRT)

    // scale key frames
    if (nodeAnimation.scaleKeyFrames.size())
        // find and interpolate scaling
        for (size_t i = this->currentScaleKey; i < nodeAnimation.scaleKeyFrames.size() - 1; i++)
        {
            // check the range we're in
            if ((nodeAnimation.scaleKeyFrames.at(i).timestamp < timeOffset) && (timeOffset <= nodeAnimation.scaleKeyFrames.at(i + 1).timestamp)) {

                // pass time values to the lambda for normalization and scaling and do a linear interpolation based on it
                scaleFactor = KeyFrameScale(nodeAnimation.scaleKeyFrames.at(i).timestamp, nodeAnimation.scaleKeyFrames.at(i + 1).timestamp);
                scale = glm::mix(nodeAnimation.scaleKeyFrames.at(i).value, nodeAnimation.scaleKeyFrames.at(i + 1).value, scaleFactor);
                this->currentScaleKey = i;
                break;
            }
        }

    // rotation key frames
    if (nodeAnimation.rotationKeyFrames.size())
        // find and interpolate rotation
        for (size_t i = this->currentRotationKey; i < nodeAnimation.rotationKeyFrames.size() - 1; i++)
        {
            // check the range we're in
            if ((nodeAnimation.rotationKeyFrames.at(i).timestamp < timeOffset) && (timeOffset <= nodeAnimation.rotationKeyFrames.at(i + 1).timestamp)) {

                // pass time values to the lambda for normalization and scaling and do a spherical linear interpolation based on it
                scaleFactor = KeyFrameScale(nodeAnimation.rotationKeyFrames.at(i).timestamp, nodeAnimation.rotationKeyFrames.at(i + 1).timestamp);
                rotate = glm::slerp(nodeAnimation.rotationKeyFrames.at(i).value, nodeAnimation.rotationKeyFrames.at(i + 1).value, scaleFactor);
                this->currentRotationKey = i;
                break;
            }
        }

    // position key frames
    if (nodeAnimation.positionKeyFrames.size())
        //find and interpolate translation
        for (size_t i = this->currentPositionKey; i < nodeAnimation.positionKeyFrames.size() - 1; i++)
        {
            // check the range we're in
            if ((nodeAnimation.positionKeyFrames.at(i).timestamp < timeOffset) && (timeOffset <= nodeAnimation.positionKeyFrames.at(i + 1).timestamp)) {

                // pass time values to the lambda for normalization and scaling and do a linear interpolation based on it
                scaleFactor = KeyFrameScale(nodeAnimation.positionKeyFrames.at(i).timestamp, nodeAnimation.positionKeyFrames.at(i + 1).timestamp);
                translate = glm::mix(nodeAnimation.positionKeyFrames.at(i).value, nodeAnimation.positionKeyFrames.at(i + 1).value, scaleFactor);
                this->currentPositionKey = i;
                break;
            }
        }

    // create matrices out of calculated transforms
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), translate);
    glm::mat4 rotation = glm::mat4_cast(rotate);
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);

    // final local animated transform
    transform = translation * rotation * scaling;

    return transform;
}

void Engine::Animation::TransformNodes(double timeOffset)
{
    // go through all nodes affected by this animation and apply the transformations
    for (size_t i = 0; i < this->nodeAnimations.size(); i++)
        this->nodeAnimations.at(i).node->SetAnimatedTransform(this->TransformNode(this->nodeAnimations.at(i), timeOffset));
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

unsigned int Engine::Animation::GetCurrentPositionKey()
{
    return this->currentPositionKey;
}

unsigned int Engine::Animation::GetCurrentRotationKey()
{
    return this->currentRotationKey;
}

unsigned int Engine::Animation::GetCurrentScaleKey()
{
    return this->currentScaleKey;
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

void Engine::Animation::SetCurrentPositionKey(unsigned int key)
{
    this->currentPositionKey = key;
}

void Engine::Animation::SetCurrentRotationKey(unsigned int key)
{
    this->currentRotationKey = key;
}

void Engine::Animation::SetCurrentScaleKey(unsigned int key)
{
    this->currentScaleKey = key;
}