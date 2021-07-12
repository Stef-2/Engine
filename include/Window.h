#ifndef WINDOW_H
#define WINDOW_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "glfw3.h"
#include "memory"
#include "iostream"

namespace Engine
{
//GLFWwindow* wrapper class
class Window
{
    public:
        GLFWwindow* GetWindow();
        GLFWwindow* GetShared();
        GLint* GetVersion();
        GLint* GetDimensions();
        GLFWmonitor* GetMonitor();
        char* GetTitle();
        void Initialize();

        Window(GLint width, GLint height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLint* version);
        Window(GLint width, GLint height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
        Window(GLint width, GLint height, const char* title);
        Window() = delete;
        ~Window();

    private:
        GLFWwindow* window;
        GLFWwindow* share;
        GLFWmonitor* monitor;
        GLint* version;
        GLint width;
        GLint height;
        char* title;
};

}

#endif // WINDOW_H
