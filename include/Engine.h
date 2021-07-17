#ifndef Engine_H
#define Engine_H

#include "window.h"
#include "Renderer.h"
#include "glfw3.h"

#include "iostream"

namespace Engine
{

//singleton Motor class,
//used to instantiate subsystems and hold data that would otherwise be global
class Motor
{
    public:
        static Motor& GetInstance();
        Window* GetWindow();
        GLint* GetResolution();
        GLint GetColorDepth();
        void Intro();
        void SetWindow(Engine::Window* window);
        void SetResolution(GLint* resolution);
        void SetColorDepth(GLint colorDepth);
        ~Motor();

    private:
        Motor() = default;
        Motor(const Motor&) = delete;
        Motor& operator=(const Motor&) = delete;
        Motor(Motor&&) = delete;
        Motor& operator=(Motor&&) = delete;

        Window* window;
        GLint* resolution;
        GLint colorDepth;
        Renderer renderer;
};

#endif // Engine_H

}
