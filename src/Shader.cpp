#include "Shader.h"

Engine::Shader* Engine::Shader::currentShader = {};

Engine::Shader::Shader()
{
    this->compileSuccess = 0;
    this->vertexShader = 0;
    this->fragmentShader = 0;
    this->programID = 0;

    this->modelTransformLocation = {};
    this->viewTransformLocation = {};
    this->projectionTransformLocation = {};
    this->BoneTransformsLocation = {};

    this->vertexPositionLocation = {};
    this->vertexNormalLocation = {};
    this->vertexUvLocation = {};
    this->vertexBoneIdLocation = {};
    this->vertexBoneWeights = {};
}

Engine::Shader::Shader(std::string vertexShader, std::string fragmentShader)
{
    this->compileSuccess = 0;
    this->vertexShader = 0;
    this->fragmentShader = 0;
    this->programID = 0;

    this->modelTransformLocation = {};
    this->viewTransformLocation = {};
    this->projectionTransformLocation = {};
    this->BoneTransformsLocation = {};

    this->vertexPositionLocation = {};
    this->vertexNormalLocation = {};
    this->vertexUvLocation = {};
    this->vertexBoneIdLocation = {};
    this->vertexBoneWeights = {};

    this->compileSuccess = this->SetVertexShader(vertexShader);
    this->compileSuccess = this->SetFragmentShader(fragmentShader);

    this->compileSuccess = this->CompileProgram();
}

int Engine::Shader::SetVertexShader(std::string filePath)
{
    std::string code;
    char c;

    int success = false;
    int length = 0;
    // char* data;

    // vertex shader read
    std::ifstream reader(filePath);
    while(!reader.eof())
    {
        reader.read(&c,1);
        code += c;
    }

    reader.close();
    const char* data = code.c_str();
    length = code.size();

    // vertex shader compile and bind
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &data, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    this->vertexShader = vertexShader;
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, this->vsLog);
        std::cerr << filePath << " - vertex shader compilation failed: " << std::endl << this->vsLog << std::endl;
        std::cerr<< "vertex shader: " << std::endl << data << ", Length: " << length << std::endl;
        // delete data;
        return 0;
    }
    else
    {
        std::cout << filePath << " vertex shader compiled successfully" << std::endl;
        return vertexShader;
    }
}

int Engine::Shader::SetFragmentShader(std::string filePath)
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
    length = code.size();

    // fragment shader compile and bind
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &data, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    this->fragmentShader = fragmentShader;
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, this->fsLog);
        std::cerr << filePath << " - fragment shader compilation failed: " << std::endl << this->fsLog << std::endl;
        std::cerr<< "fragment shader: " << std::endl << data << ", Length: " << length << std::endl;
        delete data;
        return 0;
    }
    else
    {
        std::cout << filePath << " fragment shader compiled successfully" << std::endl;
        return fragmentShader;
    }
}

int Engine::Shader::CompileProgram()
{
    int success;

    // check if both the required shaders are present
    if(!this->vertexShader) {
        std::cerr << "unable to compile shader program, vertex shader is missing" << std::endl;
        this->compileSuccess = 0;
        this->programID = 0;
        return 0;
    }
    if(!this->fragmentShader) {
        std::cerr << "unable to compile shader program, fragment shader is missing" << std::endl;
        this->compileSuccess = 0;
        this->programID = 0;
        return 0;
    }

    // main shader program
    unsigned int shaderProgram = 0;

    // compile and link
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, this->vertexShader);
    glAttachShader(shaderProgram, this->fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);

    // check for and print any errors we may have had
    if(!success) {
        
        glGetProgramInfoLog(shaderProgram, 512, NULL, spLog);
        std::cerr << "shader program linking failed: " << std::endl << spLog << std::endl;
        this->compileSuccess = 0;
        return 0;
    }
    else 
    {
        // if everything wen't smoothly...
        this->compileSuccess = 1;
        this->programID = shaderProgram;
        std::cout << "shader program compiled and linked successfully" << std::endl;

        // get and bind the location of shader attributes
        // we can use these later without having to bother the gfx card since most glGetX() functions are very slow
        this->modelTransformLocation = glGetUniformLocation(this->programID, "model");

        this->viewTransformLocation = glGetUniformLocation(this->programID, "view");

        this->projectionTransformLocation = glGetUniformLocation(this->programID, "projection");

        this->BoneTransformsLocation = glGetUniformLocation(this->programID, "boneTransformations");

        this->vertexPositionLocation = glGetAttribLocation(this->programID, "vertexPosition");
        glBindAttribLocation(this->programID, this->vertexPositionLocation, "vertexPosition");

        this->vertexNormalLocation = glGetAttribLocation(this->programID, "vertexNormal");
        glBindAttribLocation(this->programID, this->vertexNormalLocation, "vertexNormal");

        this->vertexUvLocation = glGetAttribLocation(this->programID, "vertexCoordinate");
        glBindAttribLocation(this->programID, this->vertexUvLocation, "vertexCoordinate");

        this->vertexBoneIdLocation = glGetAttribLocation(this->programID, "boneIDs");
        glBindAttribLocation(this->programID, this->vertexBoneIdLocation, "boneIDs");

        this->vertexBoneWeights = glGetAttribLocation(this->programID, "boneWeights");
        glBindAttribLocation(this->programID, this->vertexBoneWeights, "boneWeights");

        return shaderProgram;
    }
}

unsigned int Engine::Shader::GetAttributeLocation(Engine::Shader::ShaderAttribute attribute)
{
    switch (attribute)
    {
    case Engine::Shader::ShaderAttribute::MODEL_LOCATION:
        return this->modelTransformLocation;

    case Engine::Shader::ShaderAttribute::VIEW_LOCATION:
        return this->viewTransformLocation;

    case Engine::Shader::ShaderAttribute::PROJECTION_LOCATION:
        return this->projectionTransformLocation;

    case Engine::Shader::ShaderAttribute::BONE_TRANSFORMATIONS:
        return this->BoneTransformsLocation;

    case Engine::Shader::ShaderAttribute::VERTEX_POSITION_LOCATION:
        return this->vertexPositionLocation;

    case Engine::Shader::ShaderAttribute::VERTEX_NORMAL_LOCATION:
        return this->vertexNormalLocation;

    case Engine::Shader::ShaderAttribute::VERTEX_UV_LOCATION:
        return this->vertexUvLocation;

    case Engine::Shader::ShaderAttribute::VERTEX_BONE_ID_LOCATION:
        return this->vertexBoneIdLocation;

    case Engine::Shader::ShaderAttribute::VERTEX_BONE_WEIGHTS_LOCATION:
        return this->vertexBoneWeights;
    }
}

void Engine::Shader::Activate()
{
    Engine::Shader::currentShader = this;
    glUseProgram(this->programID);
}

Engine::Shader* Engine::Shader::GetCurrentShader()
{
    return Engine::Shader::currentShader;
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
