#include "Engine.h"

Engine::Motor& Engine::Motor::GetInstance()
{
    static Motor instance;

    return instance;
}

Engine::Motor::Motor()
{
    this->renderer = {};
    this->collider = {};

    // just temporary initialization to prevent garbage values from being held here
    // GetDeltaTime() and Run() functions are actually responsible for keeping these updated
    this->lastTime = 0;
    this->currentTime = glfwGetTime();
}

void Engine::Motor::Initialize()
{
    // initialize glfw
    if (!glfwInit()) {
        std::cerr << "Glfw could not be initialized." << std::endl;
        abort();
    }

    this->InitializeCallbacks();
}

void Engine::Motor::InitializeCallbacks()
{
    glfwSetErrorCallback(Engine::Motor::ErrorCallback);
    glfwSetKeyCallback(this->window.GetGlWindow(), Engine::Motor::KeyCallback);
    glfwSetFramebufferSizeCallback(this->window.GetGlWindow(), Engine::Motor::FrameBufferCallback);
    glfwSetCursorPosCallback(this->window.GetGlWindow(), Engine::Motor::MouseCallback);
    glfwSetScrollCallback(this->window.GetGlWindow(), Engine::Motor::ScrollCallback);
}

double Engine::Motor::GetDeltaTime()
{
    return this->currentTime - this->lastTime;;
}

void Engine::Motor::SetDeltaTime()
{
    this->currentTime = glfwGetTime();
    this->lastTime = this->currentTime;
}

Engine::Window& Engine::Motor::GetWindow()
{
    return this->window;
}

Engine::Renderer& Engine::Motor::GetRenderer()
{
    return this->renderer;
}

Engine::Collider& Engine::Motor::GetCollider()
{
    return this->collider;
}

void Engine::Motor::SetWindow(Engine::Window& window)
{
    this->window = window;
}

void Engine::Motor::Intro()
{

}

void Engine::Motor::Run()
{

}

void Engine::Motor::ErrorCallback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void Engine::Motor::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        std::cout << "Window closure requested." << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Engine::Motor::FrameBufferCallback(GLFWwindow* window, int width, int height)
{
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void Engine::Motor::MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    float lastX = 1280 / 2, lastY = 720 / 2;
    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos;
    float yaw = 0, pitch = 0;
    lastX = xPos;
    lastY = yPos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // camera.RotateAbsolute(direction.x, direction.y, direction.z);
}

void Engine::Motor::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    // camera.SetFov(camera.GetFov() - (float)yOffset);
}