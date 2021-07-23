#ifndef Engine_H
#define Engine_H

#include "window.h"
#include "Renderer.h"
#include "Collider.h"
#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glfw3.h"

#include "iostream"

namespace Engine
{

    //Motor (Engine) class, named so as not to be confused with the project's namespace
    //the class is designed as a singleton for three reasons:
    //1. it is the outermost class of the program, encapsulating everything else. At no point does it make any sense to have multiple instances of it
    //2. it holds much of the high level engine settings which would be cumbersome to constantly pass around if they were local to a single object
    //3. it employs glfw callbacks which, being C functions are unaware of objects or this->pointer and thus have to be either fully global or static functions

    //the engine instantiates Renderer and Collider subsystems, glfw's hardware input and error handling callbacks
    //it can hold any number of Engine::Window objects, which create and initialize OpenGL contexts for rendering
    //the class holds general engine related options and data;
    //finaly, it employs the Run() function which serves as the main engine loop
    class Motor
    {
        public:
            //singleton instance provider
            static Motor& GetInstance();

            //make sure it can't be copy constructed
            Motor(Motor const&) = delete;
            void operator=(Motor const&) = delete;

            //wrapper class for OpenGL window struct
            //holds requested and given OpenGL version -
            //due to the way OpenGL likes to couple that data with it's window and context
            Engine::Window& GetWindow();
            void SetWindow(Engine::Window& window);

            //rendering subsystem
            Engine::Renderer& GetRenderer();

            //physics simulation subsystem
            Engine::Collider& GetCollider();

            void Initialize();

            //error info handling
            static void ErrorCallback(int error, const char* description);
            //keyboard input handling
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            //mouse scroll handling
            static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
            //mouse movement handling
            static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
            //frame buffer handling
            static void FrameBufferCallback(GLFWwindow* window, int width, int height);

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
            Motor();

            //initializes glfw callbacks
            //being a C library with no knowledge of objects or this->pointer,
            //glfw can only accept static or fully global functions as valid callbacks
            void InitializeCallbacks();

            Window window;
            Renderer renderer;
            Collider collider;
            double currentTime;
            double lastTime;
    };

}

#endif // Engine_H