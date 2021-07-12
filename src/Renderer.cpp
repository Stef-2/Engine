#include "Renderer.h"

Engine::Renderer::Renderer()
{
    //ctor
}

Engine::Renderer::~Renderer()
{
    //dtor
}

int Engine::Renderer::DeltaTime()
{
    float deltaTime;

    this->currentFrame = glfwGetTime();
    deltaTime = this->currentFrame - this->lastFrame;
    this->lastFrame = this->currentFrame;

    return deltaTime;
}
