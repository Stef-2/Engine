#include "Renderer.h"

Engine::Renderer::Renderer()
{
    this->colorDepth = 24;
}

void Engine::Renderer::Render(Engine::Camera& camera, std::vector<Engine::Actor>* actors)
{
    //bounding boxes culling
    glm::vec4* frustumPlanes = camera.GetFrustumPlanes();

    //go through all the actors
    for (size_t i = 0; i < actors->size(); i++)
    {
        //go through all 6 camera frustum planes
        for (size_t j = 0; j < 6; j++)
        {

        }
    }
}

void Engine::Renderer::SetColorDepth(int colorDepth)
{
    this->colorDepth = colorDepth;
}
int Engine::Renderer::GetColorDepth()
{
    return this->colorDepth;
}
