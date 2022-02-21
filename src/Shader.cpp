#include "Shader.h"

Engine::Shader* Engine::Shader::currentShader = {};

unsigned int Engine::Shader::mvpBlock = -1;
unsigned int Engine::Shader::materialParametersBlock = -1;
unsigned int Engine::Shader::pointLightsBLock = -1;
unsigned int Engine::Shader::spotLightsBLock = -1;
unsigned int Engine::Shader::directionalLightsBLock = -1;
unsigned int Engine::Shader::ambientLightsBLock = -1;

Engine::Shader::Shader()
{
    this->compileSuccess = 0;
    this->vertexShader = 0;
    this->geometryShader = 0;
    this->fragmentShader = 0;
    this->programID = 0;
    this->shaderFlags = Engine::Shader::ShaderFlag(0u);

    this->modelTransformLocation = {};
    this->viewTransformLocation = {};
    this->projectionTransformLocation = {};
    this->BoneTransformsLocation = {};

    this->vertexPositionLocation = {};
    this->vertexNormalLocation = {};
    this->vertexUvLocation = {};
    this->vertexBoneIdLocation = {};
    this->vertexBoneWeights = {};

    this->diffuseMapLocation = {};
    this->roughnessMapLocation = {};
    this->metallicMapLocation = {};
    this->specularMapLocation = {};
    this->normalMapLocation = {};
    this->alphaMapLocation = {};
    this->cubeMapLocation = {};
    this->shadowMapsLocation = {};
}

Engine::Shader::Shader(std::string vertexShader, std::string fragmentShader)
{
    this->compileSuccess = 0;
    this->vertexShader = 0;
    this->geometryShader = 0;
    this->fragmentShader = 0;
    this->programID = 0;
    this->shaderFlags = Engine::Shader::ShaderFlag(0u);

    this->modelTransformLocation = {};
    this->viewTransformLocation = {};
    this->projectionTransformLocation = {};
    this->BoneTransformsLocation = {};

    this->vertexPositionLocation = {};
    this->vertexNormalLocation = {};
    this->vertexUvLocation = {};
    this->vertexBoneIdLocation = {};
    this->vertexBoneWeights = {};

    this->diffuseMapLocation = {};
    this->roughnessMapLocation = {};
    this->metallicMapLocation = {};
    this->specularMapLocation = {};
    this->normalMapLocation = {};
    this->alphaMapLocation = {};
    this->cubeMapLocation = {};

    this->compileSuccess = this->SetVertexShader(vertexShader);
    this->compileSuccess = this->SetFragmentShader(fragmentShader);

    this->compileSuccess = this->CompileProgram();
}

Engine::Shader::Shader(std::string vertexShader, std::string geometryShader, std::string fragmentShader)
{
    this->compileSuccess = 0;
    this->vertexShader = 0;
    this->geometryShader = 0;
    this->fragmentShader = 0;
    this->programID = 0;
    this->shaderFlags = Engine::Shader::ShaderFlag(0u);

    this->modelTransformLocation = {};
    this->viewTransformLocation = {};
    this->projectionTransformLocation = {};
    this->BoneTransformsLocation = {};

    this->vertexPositionLocation = {};
    this->vertexNormalLocation = {};
    this->vertexUvLocation = {};
    this->vertexBoneIdLocation = {};
    this->vertexBoneWeights = {};

    this->diffuseMapLocation = {};
    this->roughnessMapLocation = {};
    this->metallicMapLocation = {};
    this->specularMapLocation = {};
    this->normalMapLocation = {};
    this->alphaMapLocation = {};
    this->cubeMapLocation = {};

    this->compileSuccess = this->SetVertexShader(vertexShader);
    this->compileSuccess = this->SetGeometryShader(geometryShader);
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
    else {
        std::cout << filePath << " vertex shader compiled successfully" << std::endl;
        return vertexShader;
    }
}

int Engine::Shader::SetGeometryShader(std::string filePath)
{
    std::string code;
    char c;

    int success = false;
    int length = 0;
    // char* data;

    // vertex shader read
    std::ifstream reader(filePath);
    while (!reader.eof())
    {
        reader.read(&c, 1);
        code += c;
    }

    reader.close();
    const char* data = code.c_str();
    length = code.size();

    // vertex shader compile and bind
    unsigned int geometryShader;
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &data, NULL);
    glCompileShader(geometryShader);
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    this->geometryShader = geometryShader;

    if (!success) {
        glGetShaderInfoLog(geometryShader, 512, NULL, this->vsLog);
        std::cerr << filePath << " - geometry shader compilation failed: " << std::endl << this->gsLog << std::endl;
        std::cerr << "geometry shader: " << std::endl << data << ", Length: " << length << std::endl;
        // delete data;
        return 0;
    }
    else {
        std::cout << filePath << " geometry shader compiled successfully" << std::endl;
        return geometryShader;
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
    else {
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

    if (this->vertexShader)
    glAttachShader(shaderProgram, this->vertexShader);

    if (this->geometryShader)
        glAttachShader(shaderProgram, this->geometryShader);

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

        // get and bind the location of shader attributes and uniforms
        // we can use these later without having to bother the gfx card since most glGetX() functions are very slow

        // basic vertex shader input
        this->vertexPositionLocation = glGetAttribLocation(this->programID, "vertexPosition");
        this->vertexNormalLocation = glGetAttribLocation(this->programID, "vertexNormal");
        this->vertexUvLocation = glGetAttribLocation(this->programID, "vertexCoordinate");

        // basic MVP matrices with separate input
        this->modelTransformLocation = glGetUniformLocation(this->programID, "model");
        this->viewTransformLocation = glGetUniformLocation(this->programID, "view");
        this->projectionTransformLocation = glGetUniformLocation(this->programID, "projection");

        // extended vertex shader inputs for animated meshes
        this->BoneTransformsLocation = glGetUniformLocation(this->programID, "boneTransformations");
        this->vertexBoneIdLocation = glGetAttribLocation(this->programID, "boneIDs");
        this->vertexBoneWeights = glGetAttribLocation(this->programID, "boneWeights");

        // shader flags for uber shader
        this->shaderFlagsLocation = glGetUniformLocation(this->programID, "shaderFlags");

        // uniform samplers for material textures
        this->diffuseMapLocation = glGetUniformLocation(this->programID, "diffuseMap");
        this->roughnessMapLocation = glGetUniformLocation(this->programID, "roughnessMap");
        this->metallicMapLocation = glGetUniformLocation(this->programID, "metallicMap");
        this->specularMapLocation = glGetUniformLocation(this->programID, "specularMap");
        this->normalMapLocation = glGetUniformLocation(this->programID, "normalMap");
        this->alphaMapLocation = glGetUniformLocation(this->programID, "alphaMap");
        this->cubeMapLocation = glGetUniformLocation(this->programID, "cubeMap");
        this->shadowMapsLocation = glGetUniformLocation(this->programID, "shadowMaps");

        // generate and bind uniform buffer objects and shader storage buffer objects if they're present in the shader(s)
        // this initialization needs to happen only once for each one of these as they are global in GPU memory
        
        // model-view-projection (MVP) matrices
        if (int(glGetUniformBlockIndex(shaderProgram, "mvpMatrices")) >= 0 &&
            !(this->mvpBlock >= 0 && this->mvpBlock < unsigned(-1)))
        {
            glGenBuffers(1, &this->mvpBlock);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->mvpBlock);
            glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
            
        }

        // material parameters
        if (int(glGetUniformBlockIndex(shaderProgram, "materialParameters")) >= 0 &&
            !(this->materialParametersBlock >= 0 && this->materialParametersBlock < unsigned(-1)))
        {
            // struct that matches the shader material parameters uniform buffer object
            // to be used as sizeof() and offsetof() reference in data alignment and size matching
            struct MaterialParametersReference
            {
                glm::vec3 diffuse;
                float roughness;
                float metallic;

                float specular;
                glm::vec3 normal;
                float alpha;
            };

            glGenBuffers(1, &this->materialParametersBlock);
            glBindBufferBase(GL_UNIFORM_BUFFER, 5, this->materialParametersBlock);
            glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(MaterialParametersReference), NULL, GL_DYNAMIC_DRAW);
        }

        // point lights
        if (int(glGetProgramResourceIndex(shaderProgram, GL_SHADER_STORAGE_BLOCK, "PointLights")) >= 0 && 
            !(this->pointLightsBLock >= 0 && this->pointLightsBLock < unsigned(-1)))
        {
            glGenBuffers(1, &this->pointLightsBLock);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->pointLightsBLock);
        }
        
        // spot lights
        if (int(glGetProgramResourceIndex(shaderProgram, GL_SHADER_STORAGE_BLOCK, "SpotLights")) >= 0 &&
            !(this->spotLightsBLock >= 0 && this->spotLightsBLock < unsigned(-1)))
        {
            glGenBuffers(1, &this->spotLightsBLock);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->spotLightsBLock);
        }

        // directional lights
        if (int(glGetProgramResourceIndex(shaderProgram, GL_SHADER_STORAGE_BLOCK, "DirectionalLights")) >= 0 &&
            !(this->directionalLightsBLock >= 0 && this->directionalLightsBLock < unsigned(-1)))
        {
            glGenBuffers(1, &this->directionalLightsBLock);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, this->directionalLightsBLock);
        }

        // ambient lights
        if (int(glGetProgramResourceIndex(shaderProgram, GL_SHADER_STORAGE_BLOCK, "AmbientLights")) >= 0 &&
            !(this->ambientLightsBLock >= 0 && this->ambientLightsBLock < unsigned(-1)))
        {
            glGenBuffers(1, &this->ambientLightsBLock);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, this->ambientLightsBLock);
        }

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

    case Engine::Shader::ShaderAttribute::SHADER_PARAMETERS:
        return this->shaderFlagsLocation;

    case Engine::Shader::ShaderAttribute::DIFFUSE_MAP:
        return this->diffuseMapLocation;

    case Engine::Shader::ShaderAttribute::ROUGHNESS_MAP:
        return this->roughnessMapLocation;

    case Engine::Shader::ShaderAttribute::METALLIC_MAP:
        return this->metallicMapLocation;

    case Engine::Shader::ShaderAttribute::SPECULAR_MAP:
        return this->specularMapLocation;

    case Engine::Shader::ShaderAttribute::NORMAL_MAP:
        return this->normalMapLocation;

    case Engine::Shader::ShaderAttribute::ALPHA_MAP:
        return this->alphaMapLocation;

    case Engine::Shader::ShaderAttribute::CUBE_MAP:
        return this->cubeMapLocation;

    case Engine::Shader::ShaderAttribute::SHADOW_MAPS:
        return this->shadowMapsLocation;

    default:
        break;
    }
}

Engine::Shader::ShaderFlag& Engine::Shader::GetShaderFlags()
{
    return this->shaderFlags;
}

bool Engine::Shader::GetShaderFlag(Engine::Shader::ShaderFlag flag)
{
    return bool(unsigned int(this->shaderFlags) & unsigned int(flag));
}

unsigned int Engine::Shader::GetUniformBuffer(Engine::Shader::UniformBuffer buffer)
{
    switch (buffer)
    {
    case Engine::Shader::UniformBuffer::MVP_MATRICES:
        return Engine::Shader::mvpBlock;

    case Engine::Shader::UniformBuffer::POINT_LIGHTS:
        return Engine::Shader::pointLightsBLock;

    case Engine::Shader::UniformBuffer::SPOT_LIGHTS:
        return Engine::Shader::spotLightsBLock;

    case Engine::Shader::UniformBuffer::DIRECTIONAL_LIGHTS:
        return Engine::Shader::directionalLightsBLock;

    case Engine::Shader::UniformBuffer::AMBIENT_LIGHTS:
        return Engine::Shader::ambientLightsBLock;

    case Engine::Shader::UniformBuffer::MATERIAL_PARAMETERS:
        return Engine::Shader::materialParametersBlock;

    default:
        break;
    }
}

void Engine::Shader::SetShaderFlag(Engine::Shader::ShaderFlag flag)
{
    this->shaderFlags = flag;

    glUniform1ui(this->shaderFlagsLocation, unsigned int(this->shaderFlags));
}

void Engine::Shader::Activate()
{
    Engine::Shader::currentShader = this;
    glUseProgram(this->programID);
}

Engine::Shader& Engine::Shader::GetCurrentShader()
{
    return *Engine::Shader::currentShader;
}

std::string Engine::Shader::GetLogData()
{
    std::string vs(this->vsLog);
    std::string gs(this->gsLog);
    std::string fg(this->fsLog);
    std::string sp(this->spLog);

    std::string log = "vertex Shader log: \n" + vs;
    log += "\n geometry shader log: \n" + gs;
    log += "\n fragment Shader log: \n" + fg;
    log += "\n shader program log: \n" + sp;

    return log;
}

unsigned int Engine::Shader::GetVertexShader()
{
    return this->vertexShader;
}

unsigned int Engine::Shader::GetGeometryShader()
{
    return this->geometryShader;
}

unsigned int Engine::Shader::GetFragmentShader()
{
    return this->fragmentShader;
}

unsigned int Engine::Shader::GetProgramID()
{
    return this->programID;
}
