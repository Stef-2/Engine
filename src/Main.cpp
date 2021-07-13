#include "Main.h"

glm::vec3 cameraFront;
float fov = 45;

int main()
{
    if (!Engine::Initialize())
        glfwTerminate();


    Engine::Motor& motor = Engine::Motor::GetInstance();

    Engine::Camera camera(new GLfloat[3]{0.0,0.0,0.0});
    std::string title;
    motor.SetWindow(new Engine::Window(1280, 720, "Engine", NULL, NULL, new int[2] {2,1}));

    Engine::Object* activeObject;

    activeObject = &camera;

    Engine::InitializeCallbacks(&motor);

    //-----------------------------------------------
  
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float cameraSpeed;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    Engine::Object obj;

    obj.SetShader(new Engine::Shader("C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\vertex shader.vs", "C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\fragment shader.fs"));
    obj.SetModel(new Engine::Model("C:\\Users\\Cofara\\source\\repos\\Engine\\resources\\MaleLow.obj"));

    //texture
    int width, height, nrChannels;
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data = stbi_load("C:\\Users\\Cofara\\source\\repos\\Engine\\resources\\wood.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    //stbi_image_free(data);

    //-----------------------------------------------
    glEnable(GL_DEPTH_TEST);

    //!!!-------------- main loop --------------!!!
    while(!glfwWindowShouldClose(motor.GetWindow()->GetWindow()))
    {
        //theres a whole bunch of overlap in here, gotta cleanup

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cameraSpeed = 2.5f * deltaTime;

        if (glfwGetKey(motor.GetWindow()->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(motor.GetWindow()->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(motor.GetWindow()->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(motor.GetWindow()->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(motor.GetWindow()->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraUp;
        if (glfwGetKey(motor.GetWindow()->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            cameraPos += cameraSpeed * -cameraUp;

        title = "Engine --- Position: X: " + std::to_string(cameraPos.x) + " --- Y: " + std::to_string(cameraPos.y) + " --- Z: " + std::to_string(cameraPos.z);

        glfwSetWindowTitle(motor.GetWindow()->GetWindow(), title.c_str());
        glfwSwapBuffers(motor.GetWindow()->GetWindow());
        glfwPollEvents();

        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(fov), (float)motor.GetWindow()->GetDimensions()[0] / (float)(float)motor.GetWindow()->GetDimensions()[1], 0.1f, 10000.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        obj.Draw(view, projection);

        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
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
    cameraFront = glm::normalize(direction);
}

void Engine::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    fov -= (float)yOffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
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
    glfwSetErrorCallback(ErrorCallback);
    glfwSetKeyCallback(motor->GetWindow()->GetWindow(), KeyCallback);
    glfwSetFramebufferSizeCallback(motor->GetWindow()->GetWindow(), FrameBufferCallback);
    glfwSetCursorPosCallback(motor->GetWindow()->GetWindow(), MouseCallback);
    glfwSetScrollCallback(motor->GetWindow()->GetWindow(), ScrollCallback);
}

void Engine::CubeSetup(float* verts, unsigned int* indices, const char* vb, const char* fb)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vb, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fb, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //vertex buffer
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glBindAttribLocation(shaderProgram, 0, "vertPos");
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glBindAttribLocation(shaderProgram, 1, "vertColor");
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindAttribLocation(shaderProgram, 2, "vertCoord");
    glEnableVertexAttribArray(2);

    //element buffer
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glUseProgram(shaderProgram);
}
