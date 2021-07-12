#ifndef RENDERER_H
#define RENDERER_H

#include "glfw3.h"

namespace Engine
{
//wrapper class for general OpenGL settings and objects
class Renderer
{
    public:
        Renderer();
        ~Renderer();
        int Render();
        int DeltaTime();


    private:

        int colorDepth;
        float currentFrame;
        float lastFrame;
};

}
#endif // RENDERER_H
