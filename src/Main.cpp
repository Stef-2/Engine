#include "Main.h"

glm::vec3 cameraFront;
float fov = 45;

int main()
{
    if (!Engine::Initialize())
        glfwTerminate();


    Engine::Motor& motor = Engine::Motor::GetInstance();

    Engine::Camera camera(new GLfloat[3]{0.0,0.0,0.0});

    motor.SetWindow(new Engine::Window(1280, 720, "Engine", NULL, NULL, new int[2] {2,1}));

    Engine::Object* activeObject;

    activeObject = &camera;

    Engine::InitializeCallbacks(&motor);

    //-----------------------------------------------
    float verts[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
                                };

    unsigned int indices[] = {0, 1, 3,
                              1, 2, 3};




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

    unsigned int vbo;
    unsigned int ebo;

    int success;
    char infoLog1[512];
    char infoLog2[512];
    char infoLog3[512];

    //vertex shader read
    std::ifstream reader("C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\vertex shader.vs");
    reader.seekg (0, reader.end);
    int length = reader.tellg();
    reader.seekg (0, reader.beg);
    char* vb = new char [length];
    reader.read(vb, length);
    reader.close();
    std::cout<< "vertex shader: " << std::endl << vb << length << std::endl;

    //vertex shader compile and bind
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vb, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog1);
        std::cout << "vertex shader compilation failed: " << infoLog1 << std::endl;
    }

    //fragment shader read
    reader.open("C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\fragment shader.fs");
    reader.seekg (0, reader.end);
    length = reader.tellg();
    reader.seekg (0, reader.beg);
    char * fb = new char [length];
    reader.read(fb, length);
    reader.close();
    std::cout<< "fragment shader: " << std::endl << fb << std::endl;

    //fragment shader compile and bind
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fb, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
        std::cout << "fragment shader compilation failed: " << infoLog2 << std::endl;
    }

    //main shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog3);
        std::cout << "shader program linking failed: " << infoLog3 << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //vertex buffer
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
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Engine::Object obj(new float[3]{0.0f, 0.0f, 0.0f});

    //obj.SetShader(new Engine::Shader("C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\vertex shader.vs", "C:\\Users\\Cofara\\source\\repos\\Engine\\shaders\\fragment shader.fs"));
    //obj.SetModel(new Engine::Model("C:\\Users\\Cofara\\source\\repos\\Engine\\resources\\MaleLow.obj"));

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

    //std::cout << "vert attrib loc: " << vertAttribLoc << std::endl;
    //std::cout << "Image: " << data << std::endl;
    //-----------------------------------------------

    //for(int i = 0; i < 16 ; i++)
        //std::cout << *(value_ptr(ones)+i) << ", ";
    //std::cout << transformLoc << " --- " << "" << std::endl;
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //!!!-------------- main loop --------------!!!//
    std::cout << "--------------------------------------------------------------------------------------" << std::endl;

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

        glfwSwapBuffers(motor.GetWindow()->GetWindow());
        glfwPollEvents();
        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(fov), (float)motor.GetWindow()->GetDimensions()[0] / (float)(float)motor.GetWindow()->GetDimensions()[1], 0.1f, 10000.0f);

        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //set camera based on input
        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        //go through objects and draw them
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);

        }

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
