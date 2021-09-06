#ifndef WINDOW_H
#define WINDOW_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "glfw3.h"
#include "glm/vec2.hpp"
#include "memory"
#include "iostream"
#include "string"

namespace Engine
{

// GLFWwindow* wrapper class
class Window
{
    public:
        Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, glm::ivec2 version);
        Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
        Window(int width, int height, const char* title);
        Window();
        ~Window();

        GLFWwindow* GetGlWindow();
        GLFWwindow* GetShared();
        GLFWmonitor* GetMonitor();
        std::string GetTitle();
        double GetAspectRatio();
        glm::ivec2 GetRequestedVersion();
        std::string GetGivenVersion();
        glm::ivec2 GetDimensions();

        void SetWidth(int width);
        void SetHeight(int height);
        void SetTitle(std::string newTitle);
        void SetMonitor(GLFWmonitor* monitor);
        void SetShare(GLFWwindow* share);
        void SetVersion(glm::ivec2 version);
        void Initialize();

    private:
        // glfw's own window struct
        GLFWwindow* openGlWindow;
        // slots for windows with which we can share content
        GLFWwindow* share;
        // monitor associated with the window
        GLFWmonitor* monitor;
        // the requested OpenGL version
        glm::ivec2 openGlVersion;
        // OpenGL isn't guaranted to give us the version we asked for, we'll store the one we're given here
        std::string openGlGivenVersion;
        int width;
        int height;
        // OpenGL window title
        std::string title;
};

}

#endif //  WINDOW_H
