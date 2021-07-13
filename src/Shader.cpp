#include "Shader.h"

Engine::Shader::Shader()
{
    this->compileSuccess = 0;
    this->vertexShader = 0;
    this->fragmentShader = 0;
    this->programID = 0;
}

Engine::Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
    this->SetVertexShader(vertexShader);
    this->SetFragmentShader(fragmentShader);

    this->CompileProgram();
}

Engine::Shader::~Shader()
{
    //if (this->vsLog != NULL) delete this->vsLog;
    //if (this->fsLog != NULL) delete this->fsLog;
    //if (this->spLog != NULL) delete this->spLog;
}

Engine::Shader::Shader(const Shader& other)
{
    //copy ctor
}

Engine::Shader& Engine::Shader::operator=(const Shader& rhs)
{
    if (this == &rhs) return *this;

    return *this;
}

int Engine::Shader::SetVertexShader(const char* filePath)
{
    std::string code;
    char c;

    int success = false;
    int length = 0;
    //char* data;

    //vertex shader read
    std::ifstream reader(filePath);
    while(!reader.eof())
    {
        reader.read(&c,1);
        code += c;
    }

    reader.close();
    const char* data = code.c_str();

    //vertex shader compile and bind
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &data, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    this->vertexShader = vertexShader;
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, this->vsLog);
        std::cerr << "vertex shader compilation failed: " << this->vsLog << std::endl;
        std::cerr<< "vertex shader: " << std::endl << data << ", Length: " << length << std::endl;
        //delete data;
        return 0;
    }
    else
    {
        std::cout << "vs compiled successfully: " << data << std::endl;
        return vertexShader;
    }
}

int Engine::Shader::SetFragmentShader(const char* filePath)
{
    std::string code;
    char c;
    int success = false;
    int length = 0;

    std::ifstream reader(filePath);

    while(!reader.eof())
    {
        reader.read(&c, 1);
        code += c;
    }
    reader.close();
    const char* data = code.c_str();


    //fragment shader compile and bind
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &data, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    this->fragmentShader = fragmentShader;
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, this->fsLog);
        std::cerr << "fragment shader compilation failed: " << this->fsLog << std::endl;
        std::cerr<< "fragment shader: " << std::endl << data << ", Length: " << length << std::endl;
        delete data;
        return 0;
    }
    else
    {
        std::cout << "fs compiled successfully: " << data << std::endl;
        return fragmentShader;
    }
}

int Engine::Shader::CompileProgram()
{
    int success;

    //check if both the required shaders are present
    if(!this->vertexShader){
        std::cerr << "unable to compile shader program, vertex shader is missing" << std::endl;
        this->compileSuccess = 0;
        this->programID = 0;
        return 0;
    }
    if(!this->fragmentShader){
        std::cerr << "unable to compile shader program, fragment shader is missing" << std::endl;
        this->compileSuccess = 0;
        this->programID = 0;
        return 0;
    }

    //main shader program
    unsigned int shaderProgram;

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, this->vertexShader);
    glAttachShader(shaderProgram, this->fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);

    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, spLog);
        std::cerr << "shader program linking failed: " << spLog << std::endl;
        this->compileSuccess = 0;
        return 0;
    }
    else {
        this->compileSuccess = 1;
        this->programID = shaderProgram;
        std::cout << "sp compiled successfully" << std::endl;
        return shaderProgram;
    }
}

void Engine::Shader::Run()
{
    glUseProgram(this->programID);
}

std::string Engine::Shader::GetLogData()
{
    std::string vs(this->vsLog);
    std::string fg(this->fsLog);
    std::string sp(this->spLog);

    std::string log = "vertex Shader log: \n" + vs;
    log += "\n fragment Shader log: \n" + fg;
    log += "\n shader program log: \n" + sp;

    return log;
}

unsigned int Engine::Shader::GetVertexShader()
{
    return this->vertexShader;
}

unsigned int Engine::Shader::GetFragmentShader()
{
    return this->fragmentShader;
}

unsigned int Engine::Shader::GetProgramID()
{
    return this->programID;
}
