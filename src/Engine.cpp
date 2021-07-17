#include "Engine.h"

Engine::Motor& Engine::Motor::GetInstance()
{
    static Engine::Motor* motor = new Engine::Motor();
    return *motor;
}

Engine::Window* Engine::Motor::GetWindow()
{
    return this->window;
}

GLint* Engine::Motor::GetResolution()
{
    return new int[2] {this->resolution[0], this->resolution[1]};
}

GLint Engine::Motor::GetColorDepth()
{
    return this->colorDepth;
}

void Engine::Motor::SetWindow(Engine::Window* window)
{
    this->window = window;
    this->resolution = window->GetDimensions();
}

void Engine::Motor::SetResolution(GLint* resolution)
{
    this->resolution = resolution;
}

void Engine::Motor::SetColorDepth(GLint colorDepth)
{
    this->colorDepth = colorDepth;
}

//in-engine animation that plays when the program is started
void Engine::Motor::Intro()
{

}

Engine::Motor::~Motor()
{
    if (window != NULL) delete window;
    if (resolution != NULL) delete resolution;
}
