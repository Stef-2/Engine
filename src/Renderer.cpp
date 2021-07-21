#include "Renderer.h"

Engine::Renderer::Renderer()
{
    this->colorDepth = 24;
}

void Engine::Renderer::Render()
{

}

void Engine::Renderer::SetColorDepth(int colorDepth)
{
    this->colorDepth = colorDepth;
}
int Engine::Renderer::GetColorDepth()
{
    return this->colorDepth;
}
