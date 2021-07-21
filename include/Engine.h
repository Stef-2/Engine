#ifndef Engine_H
#define Engine_H

#include "window.h"
#include "Renderer.h"
#include "Collider.h"
#include "glm/vec2.hpp"
#include "glfw3.h"

#include "iostream"

namespace Engine
{

    //Motor (Engine) class, named so as not to be confused with the project's namespace
    //used to instantiate Renderer and Collider subsystems and hold general engine related options and data
    class Motor
    {
        public:
            Motor();
            Window& GetWindow();
            float GetDeltaTime();

            void Intro();
            void SetWindow(Engine::Window& window);

        private:
            Window window;

            float currentTime;
            float lastTime;

            Renderer renderer;
            Collider collider;
    };

}

#endif // Engine_H