#ifndef WINDOW_H
#define WINDOW_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "glfw3.h"
#include "memory"
#include "iostream"
#include "string"

namespace Engine
{
//GLFWwindow* wrapper class
class Window
{
    public:
        Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, int* version);
        Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
        Window(int width, int height, const char* title);
        Window() = delete;
        ~Window();

        GLFWwindow* GetWindow();
        GLFWwindow* GetShared();
        int* GetVersion();
        int* GetDimensions();
        GLFWmonitor* GetMonitor();
        std::string GetTitle();
        void SetTitle(std::string newTitle);
        void Initialize();

    private:
        GLFWwindow* window;
        GLFWwindow* share;
        GLFWmonitor* monitor;
        int* version;
        int width;
        int height;
        std::string title;
};

}

#endif // WINDOW_H
