#include "Main.h"

Engine::Camera camera(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

int main()
{
    if (!Engine::Initialize())
        glfwTerminate();

    Engine::Motor& motor = Engine::Motor::GetInstance();

    
    Engine::Window window(1280, 720, "Engine", NULL, NULL, glm::ivec2(2, 1));
    motor.SetWindow(window);

    Engine::Object::SetActiveObject(&camera);
    camera.Setup(1.0f, motor.GetWindow().GetAspectRatio(), 0.1f, 1000.0f, 45.0f);
    camera.SetUpDirection(glm::vec3(0.0f, 1.0f, 0.0f));
    Engine::InitializeCallbacks(&motor);

    //-----------------------------------------------
    Engine::OcTree<int> octree(3);
    octree.child.mins = { 0.0f, 0.0f, 0.0f };
    octree.child.maxs = { 5.0f, 6.0f, 8.0f };
    octree.Subdivide();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float lastTime = 0.0f;
    float frameMs = 0.0f;
    int nFrames = 0;
    int fps = 0;

    Engine::Actor obj1;
    obj1.SetShader(Engine::Shader("C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\vertex shader.vs",
                                     "C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\fragment shader.fs"));
    obj1.SetModel(Engine::Model("C:\\Users\\Cofara\\source\\repos\\Engine\\resources\\MaleLow.obj"));

    obj1.GetModel()->LoadMaterial(Engine::Material());
    obj1.GetModel()->GetMaterials()->at(0).SetDiffuse("C:\\Users\\Cofara\\source\\repos\\Engine\\resources\\midPoly human\\Body_Colour.jpg");
    
    Engine::Actor obj2;
    obj2.SetShader(Engine::Shader("C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\vertex shader.vs",
        "C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\fragment shader.fs"));
    obj2.SetModel(Engine::Model("C:\\Users\\Cofara\\source\\repos\\Engine\\resources\\midPoly human\\Body_Posed.obj"));

    obj2.GetModel()->LoadMaterial(Engine::Material());
    obj2.GetModel()->GetMaterials()->at(0).SetDiffuse("C:\\Users\\Cofara\\source\\repos\\Engine\\resources\\midPoly human\\Body_Subdermal.jpg");
    obj2.MoveRelative(3.0f, 0.0f, 0.0f);
    //obj1.MoveRelative(3.0f, 0.0f, 0.0f);

    //-----------------------------------------------
    glEnable(GL_DEPTH_TEST);
    lastTime = glfwGetTime();
    //!!!-------------- main loop --------------!!!
    while(!glfwWindowShouldClose(motor.GetWindow().GetGlWindow()))
    {
        //theres a whole bunch of overlap in here, gotta cleanup

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        nFrames++;

        if (currentFrame - lastTime >= 1.0f) {
            frameMs = 1000.0f / (float)nFrames;
            fps = CLOCKS_PER_SEC / deltaTime;
            nFrames = 0;
            lastTime += 1.0f;
        }

        if (glfwGetKey(motor.GetWindow().GetGlWindow(), GLFW_KEY_W) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetForwardDirection(), 0.25f);
        }
        if (glfwGetKey(motor.GetWindow().GetGlWindow(), GLFW_KEY_S) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetForwardDirection(), -0.25f);
        }
        if (glfwGetKey(motor.GetWindow().GetGlWindow(), GLFW_KEY_A) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetRightDirection(), -0.25f);
        }
        if (glfwGetKey(motor.GetWindow().GetGlWindow(), GLFW_KEY_D) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetRightDirection(), 0.25f);
        }
        if (glfwGetKey(motor.GetWindow().GetGlWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetUpDirection(), 0.25f);
        }
        if (glfwGetKey(motor.GetWindow().GetGlWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetUpDirection(), -0.25f);
        }

        motor.GetWindow().SetTitle(std::string("Engine --- Frame time: " + std::to_string(frameMs) + " ms --- FPS: " + std::to_string(fps) +
            " --- Position: X: " + std::to_string(camera.GetPosition().x) + " --- Y: " + std::to_string(camera.GetPosition().x) + " --- Z: " + std::to_string(camera.GetPosition().x) +
            " --- Rotation: X: " + std::to_string(camera.GetRotation().x) + " --- Y: " + std::to_string(camera.GetRotation().y) + " --- Z: " + std::to_string(camera.GetRotation().z)));
        
        glfwSwapBuffers(motor.GetWindow().GetGlWindow());
        glfwPollEvents();

        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //camera.Draw(&obj1);
        obj1.Draw(&camera);
        obj2.Draw(&camera);
        //obj1.RotateRelative(0.0f * deltaTime, 5.0f * deltaTime, 0.0f * deltaTime);
        obj2.RotateRelative(0.0f * deltaTime, 5.0f * deltaTime, 0.0f * deltaTime );
    }

    glfwTerminate();
}

void Engine::ErrorCallback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void Engine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        std::cout << "Window closure requested." << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Engine::FrameBufferCallback(GLFWwindow* window, int width, int height)
{
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void Engine::MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    float lastX = 1280/2, lastY = 720/2;
    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos;
    float yaw = 0, pitch = 0;
    lastX = xPos;
    lastY = yPos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    camera.RotateAbsolute(direction.x, direction.y, direction.z);
}

void Engine::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    //camera.SetFov(camera.GetFov() - (float)yOffset);
}

int Engine::Initialize()
{
    // Initialize the library
    if (!glfwInit())
        return 0;

    return 1;
}

void Engine::InitializeCallbacks(Engine::Motor* motor)
{
    //setup glfw callbacks
    glfwSetErrorCallback(ErrorCallback);
    glfwSetKeyCallback(motor->GetWindow().GetGlWindow(), KeyCallback);
    glfwSetFramebufferSizeCallback(motor->GetWindow().GetGlWindow(), FrameBufferCallback);
    glfwSetCursorPosCallback(motor->GetWindow().GetGlWindow(), MouseCallback);
    glfwSetScrollCallback(motor->GetWindow().GetGlWindow(), ScrollCallback);
}
