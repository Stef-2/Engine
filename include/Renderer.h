#ifndef RENDERER_H
#define RENDERER_H

#include "glfw3.h"

namespace Engine
{

    //Renderer class, subsystem of Engine (Motor) class
    //wraps general OpenGL settings, options and objects
    //prepares and performs rendering operations
    class Renderer
    {
    public:
        Renderer();
        void Render();

        void SetColorDepth(int colorDepth);
        int GetColorDepth();

    private:
        int colorDepth;
    };

}
#endif // RENDERER_H
