#include "Window.h"

Engine::Window::Window()
{
    this->width = {};
    this->height = {};
    this->title = {};
    this->openGlVersion = {};
    this->monitor = {};
    this->share = {};
    this->openGlWindow = {};
}

Engine::Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, glm::ivec2 version)
: width(width), height(height), title(const_cast<char*>(title)), openGlWindow(NULL), monitor(monitor), share(share), openGlVersion(version)
{
    Initialize();
}

Engine::Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
: width(width), height(height), title(const_cast<char*>(title)), openGlWindow(NULL), monitor(monitor), share(share)
{
    this->openGlVersion = glm::ivec2(4,6);
    Initialize();
}

Engine::Window::Window(int width, int height, const char* title)
: width(width), height(height), title(const_cast<char*>(title)), openGlWindow(NULL), monitor(NULL), share(NULL)
{
    this->openGlVersion = glm::ivec2(4, 6);
    Initialize();
}

void Engine::Window::Initialize()
{
    // check if the window has already been initialized
    if (!this->openGlWindow)
    {
        // check if all the required parameters have been set up
        if (this->width && this->height && this->openGlVersion.x)
        {
            // politely tell OpenGL which version of it we would like to use
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->openGlVersion.x);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->openGlVersion.y);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
            // glfwWindowHint(GLFW_DEPTH_BITS, 2);
            // glfwWindowHint(GLFW_STENCIL_BITS, 0);
            // glfwWindowHint(GLFW_ALPHA_BITS, 0);
            this->openGlWindow = glfwCreateWindow(width, height, title.c_str(), monitor, share);
            glfwMakeContextCurrent(openGlWindow);
            glfwSetInputMode(openGlWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSwapInterval(0);
            gladLoadGL();

            // OpenGL isn't required to give us the requested version, we'll store whatever it gave us
            this->openGlGivenVersion = (const char*)(glGetString(GL_VERSION));

            if (!openGlWindow || !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cerr << "OpenGL could not create a window context." << std::endl;
                glfwTerminate();
            }
        }
        else {
            std::cerr << "Some or all of the parameters needed for OpenGL Window creation are missing." << std::endl;
            glfwTerminate();
        }
    }
    else
        std::cerr << "Window already initilized." << std::endl;
}

Engine::Window::~Window()
{
    if(!openGlWindow)
        glfwDestroyWindow(openGlWindow);
    if(!share)
        glfwDestroyWindow(share);
    if(!monitor)
        delete monitor;
}

float Engine::Window::GetAspectRatio()
{
    return static_cast<float>(this->width) / static_cast<float>(this->height);
}

void Engine::Window::SetTitle(std::string newTitle)
{
    this->title = newTitle;
    glfwSetWindowTitle(this->openGlWindow, title.c_str());
}

void Engine::Window::SetWidth(int width)
{
    this->width = width;
}

void Engine::Window::SetHeight(int height)
{
    this->height = height;
}

void Engine::Window::SetVersion(glm::ivec2 version)
{
    this->openGlVersion = version;
}

void Engine::Window::SetMonitor(GLFWmonitor* monitor)
{
    this->monitor = monitor;
}

void Engine::Window::SetShare(GLFWwindow* window)
{
    this->share = window;
}

GLFWwindow* Engine::Window::GetGlWindow()
{
    return this->openGlWindow;
}

glm::ivec2 Engine::Window::GetRequestedVersion()
{
    return this->openGlVersion;
}

std::string Engine::Window::GetGivenVersion()
{
    return this->openGlGivenVersion;
}

glm::ivec2 Engine::Window::GetDimensions()
{
    return glm::ivec2(width, height);
}

std::string Engine::Window::GetTitle()
{
    return this->title;
}

GLFWmonitor* Engine::Window::GetMonitor()
{
    return this->monitor;
}

GLFWwindow* Engine::Window::GetShared()
{
    return this->share;
}
