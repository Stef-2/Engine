#include <Main.h>

Engine::Camera camera(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

float deltaTime = 0.0f;
double oldX = 1280.0 / 2.0;
double oldY = 720.0 / 2.0;

int main()
{
    if (!Engine::Initialize())
        glfwTerminate();
    
    Engine::Window window(1280, 720, "Engine", NULL, NULL, glm::ivec2(4, 6));
    Engine::Motor& engine = Engine::Motor::GetInstance();

    std::cout << "version: " << window.GetGivenVersion() << std::endl;

    engine.SetWindow(window);

    Engine::Object::SetActiveObject(&camera);
    camera.Setup(1.0f, engine.GetWindow().GetAspectRatio(), 0.1f, 1000.0f, 45.0f);
    camera.SetUpDirection(glm::vec3(0.0f, 1.0f, 0.0f));
    Engine::InitializeCallbacks(&engine);

    // -----------------------------------------------
    // Engine::OcTree<int> octree(3);;
    // octree.child.mins = { -64.0f, -64.0f, -64.0f };
    // octree.child.maxs = { 64.0f, 64.0f, 64.0f };
    // octree.Subdivide();

    unsigned int vertexCount = 0;
    unsigned int triangleCount = 0;
    unsigned int numCulls = 0;
    bool check = {};

    float lastFrame = 0.0f;
    float lastTime = 0.0f;
    float frameMs = 0.0f;
    int nFrames = 0;
    int fps = 0;

    /*Engine::Triangle tri1{ {{0.0f, 5.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                           {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                           {{8.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}} };

    Engine::Triangle tri2{ {{6.0f, 8.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                           {{6.0f, -4.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                           {{6.0f, 8.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}} };

    Engine::Triangle tri3{ {{0.0f, 5.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                           {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                           {{0.0f, -5.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}} };

    Engine::Triangle tri4{ {{6.0f, 5.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                           {{6.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                           {{6.0f, -5.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}} };

    std::string str = collider.Intersects(tri1, tri2) ? "da" : "ne";

    std::cout << str << std::endl;*/

    Engine::Shader basic(engine.GetFilePath(Engine::Motor::EngineFilePath::SHADERS_PATH).append("\\basic.vert"),
                         engine.GetFilePath(Engine::Motor::EngineFilePath::SHADERS_PATH).append("\\basic.frag"));
    
    Engine::Actor obj1;
    obj1.SetShader(basic);
    
    obj1.SetModel(Engine::Model(engine.GetFilePath(Engine::Motor::EngineFilePath::MODELS_PATH).append("\\barrel.obj")));

    obj1.GetModel()->LoadMaterial(Engine::Material());
    obj1.GetModel()->GetMaterials()->at(0).SetDiffuse(engine.GetFilePath(Engine::Motor::EngineFilePath::TEXTURES_PATH).append("\\barrel_BaseColor.png"));
    /*
    Engine::Actor obj2;
    obj2.SetShader(basic);

    obj2.SetModel(Engine::Model("C:\\Users\\Stefan\\source\\repos\\Engine\\resources\\midPoly human\\Body_Posed.obj"));

    obj2.GetModel()->LoadMaterial(Engine::Material());
    obj2.GetModel()->GetMaterials()->at(0).SetDiffuse("C:\\Users\\Stefan\\source\\repos\\Engine\\resources\\midPoly human\\Body_Subdermal.jpg");
    obj2.MoveRelative(10.0f, 0.0f, 10.0f);
    obj1.MoveRelative(0.0f, 0.0f, 0.0f);

    Engine::Actor obj3;
    obj3.SetShader(basic);

    obj3.SetModel(Engine::Model("C:\\Users\\Stefan\\source\\repos\\Engine\\resources\\Anoplophora.obj"));

    obj3.GetModel()->LoadMaterial(Engine::Material());
    obj3.GetModel()->GetMaterials()->at(0).SetDiffuse("C:\\Users\\Stefan\\source\\repos\\Engine\\resources\\Anoplophora_Diffuse.png");
    obj3.MoveRelative(-10.0f, 0.0f, -10.0f);
    */
    /*
    Engine::Actor obj1;
    obj1.SetShader(basic);

    obj1.SetModel(Engine::Model("C:\\Users\\Cofara\\source\\repos\\Engine\\resources\\PLANTS ON TABLE.fbx"));

    obj1.GetModel()->LoadMaterial(Engine::Material());
    obj1.GetModel()->GetMaterials()->at(0).SetDiffuse("C:\\Users\\Cofara\\source\\repos\\Engine\\resources\\PLANTS_ON_TABLE.jpg");
    */

    // ----------------------------------------------------------
    
    Engine::Shader skyBoxShader(engine.GetFilePath(Engine::Motor::EngineFilePath::SHADERS_PATH).append("\\skybox.vert"),
                                engine.GetFilePath(Engine::Motor::EngineFilePath::SHADERS_PATH).append("\\skybox.frag"));

    Engine::Skybox skyBox;
    skyBox.SetShader(skyBoxShader);

    std::string skyBoxTextures[6] = { engine.GetFilePath(Engine::Motor::EngineFilePath::SKYBOXES_PATH).append("\\day\\4.bmp"),
                                      engine.GetFilePath(Engine::Motor::EngineFilePath::SKYBOXES_PATH).append("\\day\\2.bmp"),
                                      engine.GetFilePath(Engine::Motor::EngineFilePath::SKYBOXES_PATH).append("\\day\\1.bmp"),
                                      engine.GetFilePath(Engine::Motor::EngineFilePath::SKYBOXES_PATH).append("\\day\\3.bmp"),
                                      engine.GetFilePath(Engine::Motor::EngineFilePath::SKYBOXES_PATH).append("\\day\\5.bmp"),
                                      engine.GetFilePath(Engine::Motor::EngineFilePath::SKYBOXES_PATH).append("\\day\\6.bmp") };

    Engine::Texture skyBoxTex(skyBoxTextures);
    skyBox.SetTexture(skyBoxTex);

    skyBox.Setup();
    // skyBox.ScaleAbsolute(10.0f, 10.0f, 10.0f);
    
    std::vector<Engine::Actor*> actors;
    actors.push_back(&obj1);
    //actors.push_back(&obj2);
    //actors.push_back(&obj3);


    for (size_t i = 0; i < actors.size(); i++)
    {
        for (size_t j = 0; j < actors.at(i)->GetModel()->GetMeshes()->size(); j++)
        {
            vertexCount += actors.at(i)->GetModel()->GetMeshes()->at(j).GetVertices()->size();
            triangleCount += actors.at(i)->GetModel()->GetMeshes()->at(j).GetTriangles().size();
        }
    }

    // -----------------------------------------------
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    lastTime = glfwGetTime();
    // !!!-------------- main loop --------------!!!
    while(!glfwWindowShouldClose(engine.GetWindow().GetGlWindow()))
    {
        // theres a whole bunch of overlap in here, gotta cleanup
        // obj1.MoveRelative(0.1f * deltaTime, 0.0f, 0.0f);
        // obj2.MoveRelative(-0.1f * deltaTime, 0.0f, 0.0f);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        nFrames++;

        if (currentFrame - lastTime >= 1.0f) {
            frameMs = 1000.0f / (float)nFrames;
            fps = (CLOCKS_PER_SEC / deltaTime) / 1000;
            nFrames = 0;
            lastTime += 1.0f;
        }

        if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_W) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetForwardDirection(), 8.0f * deltaTime);
        }
        if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_S) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetForwardDirection(), -8.0f * deltaTime);
        }
        if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_A) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetRightDirection(), -8.0f * deltaTime);
        }
        if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_D) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetRightDirection(), 8.0f * deltaTime);
        }
        if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetUpDirection(), 8.0f * deltaTime);
        }
        if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            camera.MoveRelative(camera.GetUpDirection(), -8.0f * deltaTime);
        }

        // obj2.RotateRelative(0.0f, 2.0f, 0.0f);
        // obj2.MoveRelative(0.2f, 0.0f, 0.0f);
        
        //numCulls = actors.size() - renderer.FrustumCull(camera, actors).size();
        engine.GetRenderer().Render(camera, engine.GetRenderer().FrustumCull(camera, actors));
        engine.GetRenderer().Render(camera, *obj1.GetModel()->GetBoundingBox());
        // camera.Draw(&obj1);
        // camera.Draw(&obj2);
        engine.GetRenderer().Render(camera, skyBox);

        engine.GetWindow().SetTitle(std::string("Frame time: " + std::to_string(frameMs) + " ms - FPS: " + std::to_string(fps) +
            " - Position: X: " + std::to_string(camera.GetPosition().x) + " - Y: " + std::to_string(camera.GetPosition().y) + " - Z: " + std::to_string(camera.GetPosition().z) +
            " - Rotation: X: " + std::to_string(camera.GetRotation().x) + " - Y: " + std::to_string(camera.GetRotation().y) + " - Z: " + std::to_string(camera.GetRotation().z) +
            " - number of culled objects: " + std::to_string(numCulls) + " - vertex count: " + std::to_string(vertexCount) + "- tri count: " + std::to_string(triangleCount)));
        
        glfwSwapBuffers(engine.GetWindow().GetGlWindow());
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        // obj1.Draw(&camera); // big guy (for me)
        // obj2.Draw(&camera); // small guy

        // camera.Draw(obj1.GetModel()->GetBoundingBox());
        // camera.Draw(obj2.GetModel()->GetBoundingBox());

        // obj1.RotateRelative(0.0f * deltaTime, -50.0f * deltaTime, 0.0f * deltaTime);
        // obj2.RotateRelative(0.0f * deltaTime, 50.0f * deltaTime, 0.0f * deltaTime );
        
        // camera.Draw(&octree);
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

    float sensitivity = 150;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    float difX = (xPos - oldX) * (sensitivity * deltaTime);
    float difY = (yPos - oldY) * (sensitivity * deltaTime);

    oldX = xPos;
    oldY = yPos;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 rot = glm::vec3(difY, difX, 0.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // std::cout << difX << " - " << difY << std::endl;
    // camera.RotateAbsolute(direction.x, direction.y, direction.z);
    camera.RotateRelative(rot.x, rot.y, 0.0f);
}

void Engine::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    // camera.SetFov(camera.GetFov() - (float)yOffset);
}

int Engine::Initialize()
{
    //  Initialize the library
    if (!glfwInit())
        return 0;

    return 1;
}

void Engine::InitializeCallbacks(Engine::Motor* Motor)
{
    // setup glfw callbacks
    glfwSetErrorCallback(ErrorCallback);
    glfwSetKeyCallback(Motor->GetWindow().GetGlWindow(), KeyCallback);
    glfwSetFramebufferSizeCallback(Motor->GetWindow().GetGlWindow(), FrameBufferCallback);
    glfwSetCursorPosCallback(Motor->GetWindow().GetGlWindow(), MouseCallback);
    glfwSetScrollCallback(Motor->GetWindow().GetGlWindow(), ScrollCallback);
}
