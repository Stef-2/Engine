#include "Window.h"

Engine::Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, int* version)
: width(width), height(height), title(const_cast<char*>(title)), window(NULL), monitor(monitor), share(share), version(version)
{
    Initialize();
}

Engine::Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
: width(width), height(height), title(const_cast<char*>(title)), window(NULL), monitor(monitor), share(share)
{
    Initialize();
}

Engine::Window::Window(int width, int height, const char* title)
: width(width), height(height), title(const_cast<char*>(title)), window(NULL), monitor(NULL), share(NULL)
{
    Initialize();
}

void Engine::Window::Initialize()
{
    if (!window)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version[0]);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version[1]);
        window = glfwCreateWindow(width, height, title.c_str(), monitor, share);
        glfwMakeContextCurrent(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSwapInterval(1);
        gladLoadGL();

        if (!window)
            glfwTerminate();
    }
    else
        std::cerr << "Window already initilized." << std::endl;
}

Engine::Window::~Window()
{
    if(!window)
        glfwDestroyWindow(window);
    if(!share)
        glfwDestroyWindow(share);
    delete version;
    delete monitor;
}

void Engine::Window::SetTitle(std::string newTitle)
{
    this->title = newTitle;
    glfwSetWindowTitle(this->window, title.c_str());
}

GLFWwindow* Engine::Window::GetWindow()
{
    return window;
}

int* Engine::Window::GetVersion()
{
    return version;
}
int* Engine::Window::GetDimensions()
{
    return new int[2] {width, height};
}
std::string Engine::Window::GetTitle()
{
    return title;
}
GLFWmonitor* Engine::Window::GetMonitor()
{
    return monitor;
}

GLFWwindow* Engine::Window::GetShared()
{
    return share;
}
