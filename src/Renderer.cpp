#include "Renderer.h"

Engine::Renderer::Renderer()
{
    this->colorDepth = 24;
}

unsigned int Engine::Renderer::Render(Engine::Camera& camera, std::vector<Engine::Actor*> actors)
{
    //get the frustum clipping planes from the camera
    glm::vec4* frustumPlanes = camera.GetFrustumPlanes();

    glm::vec3 front;
    
    bool visible = true;

    //number of accumulated frustum culls during this render cycle that we can use to show off
    unsigned int numCulls = 0;

    //go through all the actors
    for (size_t i = 0; i < actors.size(); i++)
    {
        //go through all 6 camera frustum planes
        for (size_t j = 0; j < 6; j++)
        {
            //assume the object is visible until proven otherwise
            visible = true;

            //find the vertex of object's bounding box that is farthest along the current frustum plane's normal
            front = actors.at(i)->GetModel()->GetBoundingBox()->mins;

            if (frustumPlanes[j].x >= 0)
                front.x = actors.at(i)->GetModel()->GetBoundingBox()->maxs.x;
            if (frustumPlanes[j].y >= 0)
                front.y = actors.at(i)->GetModel()->GetBoundingBox()->maxs.y;
            if (frustumPlanes[j].z >= 0)
                front.z = actors.at(i)->GetModel()->GetBoundingBox()->maxs.z;
            
            //if the distance from the furthest point to the current plane is less than zero, the whole object is outside the frustum
            if (glm::dot(glm::vec3(frustumPlanes[j]),front) + frustumPlanes[j].w < 0.0f) {
                visible = false;
                numCulls += 1;

                //we don't actually need to check every plane, one is enough
                //so long as we can prove the object is fully behind any one of them, we can skip the others
                break;
            }
        }

        //otherwise, the object is visible and should be rendered
        if (visible) {
            camera.Draw(actors.at(i));
            //camera.Draw(actors.at(i)->GetModel()->GetBoundingBox());
        }
    }

    return numCulls;
}

void Engine::Renderer::SetColorDepth(int colorDepth)
{
    this->colorDepth = colorDepth;
}
int Engine::Renderer::GetColorDepth()
{
    return this->colorDepth;
}
