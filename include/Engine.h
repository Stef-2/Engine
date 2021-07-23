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
    //employs the Run() function which serves the main engine loop
    class Motor
    {
        public:
            Motor();

            //wrapper class for OpenGL window struct
            //holds data on requested and given OpenGL version -
            //due to the way OpenGL likes to couple that data with it's window and context
            Engine::Window& GetWindow();
            void SetWindow(Engine::Window& window);

            //rendering subsystem
            Engine::Renderer& GetRenderer();

            //physics simulation subsystem
            Engine::Collider& GetCollider();

            //time between the previous and current engine update cycle
            //should be used as a multiplier for any rendering, movement or physics operation
            //allows us to make these operations independent from framerate
            //which is not guaranteed to be constant, depending on hardware and/or software limitation on any one system
            double GetDeltaTime();
            //updates delta time, should be used once every engine cycle
            void SetDeltaTime();

            //in-engine animation that plays when the program is started
            void Intro();

            //the main engine loop
            void Run();
            
        private:
            Window window;
            Renderer renderer;
            Collider collider;
            double currentTime;
            double lastTime; 
    };

}

#endif // Engine_H