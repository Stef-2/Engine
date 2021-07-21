#include "Engine.h"


Engine::Motor::Motor()
{
    this->renderer = {};
    this->collider = {};
}

float Engine::Motor::GetDeltaTime()
{
    float deltaTime;

    this->currentTime = glfwGetTime();
    deltaTime = this->currentTime - this->lastTime;
    this->lastTime = this->currentTime;

    return deltaTime;
}

Engine::Window& Engine::Motor::GetWindow()
{
    return this->window;
}

void Engine::Motor::SetWindow(Engine::Window& window)
{
    this->window = window;
}

//in-engine animation that plays when the program is started
void Engine::Motor::Intro()
{

}
