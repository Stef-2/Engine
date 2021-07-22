#include "Engine.h"


Engine::Motor::Motor()
{
    this->renderer = {};
    this->collider = {};

    //just temporary initialization to prevent garbage values from being held here
    //GetDeltaTime() and Run() functions are actually responsible for keeping these updated
    this->lastTime = 0;
    this->currentTime = glfwGetTime();
}

double Engine::Motor::GetDeltaTime()
{
    return this->currentTime - this->lastTime;;
}

void Engine::Motor::SetDeltaTime()
{
    this->currentTime = glfwGetTime();
    this->lastTime = this->currentTime;
}

Engine::Window& Engine::Motor::GetWindow()
{
    return this->window;
}

Engine::Renderer& Engine::Motor::GetRenderer()
{
    return this->renderer;
}

Engine::Collider& Engine::Motor::GetCollider()
{
    return this->collider;
}

void Engine::Motor::SetWindow(Engine::Window& window)
{
    this->window = window;
}

//in-engine animation that plays when the program is started
void Engine::Motor::Intro()
{

}

//main engine loop
void Engine::Motor::Run()
{

}
