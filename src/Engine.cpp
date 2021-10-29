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

    running = false;
}

void Engine::Motor::Initialize()
{
    this->InitializeCallbacks();

    // initialize the random seed with current time
    srand(std::chrono::system_clock::now().time_since_epoch().count());
}

void Engine::Motor::InitializeCallbacks()
{   /*
    glfwSetErrorCallback(Engine::Motor::ErrorCallback);
    glfwSetKeyCallback(this->window.GetGlWindow(), Engine::Motor::KeyCallback);
    glfwSetFramebufferSizeCallback(this->window.GetGlWindow(), Engine::Motor::FrameBufferCallback);
    glfwSetCursorPosCallback(this->window.GetGlWindow(), Engine::Motor::MouseCallback);
    glfwSetScrollCallback(this->window.GetGlWindow(), Engine::Motor::ScrollCallback);
    */
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(Engine::Motor::OpenGLDebugCallback, nullptr);

    // filter out notifications
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
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

std::string Engine::Motor::GetFilePath(Engine::EngineFilePath location)
{
    //engine root directory
    std::filesystem::path path = std::filesystem::current_path();

    switch (location)
    {
    case Engine::EngineFilePath::ENGINE_PATH:
        return path.string();

    case Engine::EngineFilePath::RESOURCES_PATH:
        for (const auto& dir : std::filesystem::recursive_directory_iterator(path))
            if (!dir.path().filename().string().compare("resources"))
                return dir.path().string();

            std::cerr << "could not find the resources directory" << std::endl;
            return std::string{ "" };
        

    case Engine::EngineFilePath::SHADERS_PATH:
        for (const auto& dir : std::filesystem::recursive_directory_iterator(path)) 
            if (!dir.path().filename().string().compare("shaders"))
                return dir.path().string();

            std::cerr << "could not find the shaders directory" << std::endl;
            return std::string{ "" };
        

    case Engine::EngineFilePath::TEXTURES_PATH:
        for (const auto& dir : std::filesystem::recursive_directory_iterator(path)) 
            if (!dir.path().filename().string().compare("textures"))
                return dir.path().string();

            std::cerr << "could not find the textures directory" << std::endl;
            return std::string{ "" };
        

    case Engine::EngineFilePath::MODELS_PATH:
        for (const auto& dir : std::filesystem::recursive_directory_iterator(path)) 
            if (!dir.path().filename().string().compare("models"))
                return dir.path().string();

            std::cerr << "could not find the models directory" << std::endl;
            return std::string{ "" };

    case Engine::EngineFilePath::IMAGES_PATH:
        for (const auto& dir : std::filesystem::recursive_directory_iterator(path))
            if (!dir.path().filename().string().compare("images"))
                return dir.path().string();

        std::cerr << "could not find the images directory" << std::endl;
        return std::string{ "" };

    case Engine::EngineFilePath::SKYBOXES_PATH:
        for (const auto& dir : std::filesystem::recursive_directory_iterator(path))
            if (!dir.path().filename().string().compare("skyboxes"))
                return dir.path().string();

        std::cerr << "could not find the skyboxes directory" << std::endl;
        return std::string{ "" };
        
    }
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

Engine::Animator& Engine::Motor::GetAnimator()
{
    return this->animator;
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
    this->running = true;

    while (this->running)
    {

    }
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
        Engine::Motor::GetInstance().running = false;
    }
}

void Engine::Motor::FrameBufferCallback(GLFWwindow* window, int width, int height)
{
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void Engine::Motor::OpenGLDebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:              {std::cout << "Type: Error"; abort();} break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {std::cout << "Type: Undefined Behaviour"; assert(false);} break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
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