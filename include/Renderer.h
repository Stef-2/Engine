#ifndef RENDERER_H
#define RENDERER_H

//#include "glfw3.h"
#include "Camera.h"
#include "Actor.h"

#include "vector"

namespace Engine
{

    //Renderer class, subsystem of Engine (Motor) class
    //wraps general OpenGL settings, options and objects
    //prepares and performs rendering operations
    class Renderer
    {
    public:
        Renderer();
        unsigned int Render(Engine::Camera& camera, std::vector<Engine::Actor*> actors);

        void SetColorDepth(int colorDepth);
        int GetColorDepth();

    private:
        int colorDepth;
    };

}
#endif // RENDERER_H
