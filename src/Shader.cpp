#include "Shader.h"

Engine::ShaderProgram* Engine::ShaderProgram::currentShaderProgram = {};

unsigned int Engine::ShaderProgram::mvpBlock = -1;
unsigned int Engine::ShaderProgram::materialParametersBlock = -1;
unsigned int Engine::ShaderProgram::pointLightsBLock = -1;
unsigned int Engine::ShaderProgram::spotLightsBLock = -1;
unsigned int Engine::ShaderProgram::directionalLightsBLock = -1;
unsigned int Engine::ShaderProgram::ambientLightsBLock = -1;

Engine::ShaderProgram::ShaderProgram()
{
    this->compileSuccess = 0;

    this->programID = 0;
    this->shaderFlags = Engine::ShaderProgram::ShaderFlag(0u);

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

Engine::ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(this->programID);

    if (Engine::ShaderProgram::currentShaderProgram == this)
        Engine::ShaderProgram::currentShaderProgram = nullptr;
}

Engine::ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader)
{
    this->compileSuccess = 0;
    this->programID = 0;
    this->shaderFlags = Engine::ShaderProgram::ShaderFlag(0u);

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

    this->geometryShader = {};
    this->SetVertexShader(vertexShader);
    this->SetFragmentShader(fragmentShader);

    this->compileSuccess = this->CompileProgram();
}

Engine::ShaderProgram::ShaderProgram(std::string vertexShader, std::string geometryShader, std::string fragmentShader)
{
    this->compileSuccess = 0;
    this->programID = 0;
    this->shaderFlags = Engine::ShaderProgram::ShaderFlag(0u);

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

	this->SetVertexShader(vertexShader);
	this->SetGeometryShader(geometryShader);
	this->SetFragmentShader(fragmentShader);

    this->compileSuccess = this->CompileProgram();
}

void Engine::ShaderProgram::SetVertexShader(Engine::VertexShader& shader)
{
    this->vertexShader = shader;
}

void Engine::ShaderProgram::SetVertexShader(std::string filePath)
{
    this->vertexShader.AddSource(filePath);
}

void Engine::ShaderProgram::SetGeometryShader(std::string filePath)
{
    this->geometryShader = new Engine::GeometryShader {};
    this->geometryShader->AddSource(filePath);
}

void Engine::ShaderProgram::SetFragmentShader(std::string filePath)
{
    this->fragmentShader.AddSource(filePath);
}

void Engine::ShaderProgram::SetGeometryShader(Engine::GeometryShader& shader)
{
    *this->geometryShader = shader;
}

void Engine::ShaderProgram::SetFragmentShader(Engine::FragmentShader& shader)
{
    this->fragmentShader = shader;
}

int Engine::ShaderProgram::CompileProgram()
{
    int success;

    // check if both the required shaders are present
    if(!this->vertexShader.GetShader()) {
        std::cerr << "unable to compile shader program, vertex shader is missing" << std::endl;
        this->compileSuccess = 0;
        this->programID = 0;
        return 0;
    }
    if(!this->fragmentShader.GetShader()) {
        std::cerr << "unable to compile shader program, fragment shader is missing" << std::endl;
        this->compileSuccess = 0;
        this->programID = 0;
        return 0;
    }

    // main shader program
    unsigned int shaderProgram = 0;

    // compile and link
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, this->vertexShader.GetShader());

    if (this->geometryShader)
        glAttachShader(shaderProgram, this->geometryShader->GetShader());

    glAttachShader(shaderProgram, this->fragmentShader.GetShader());

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    glDeleteShader(this->vertexShader.GetShader());
    glDeleteShader(this->fragmentShader.GetShader());

    // check for and print any errors we may have had
    if(!success) {
        
        glGetProgramInfoLog(shaderProgram, 512, NULL, this->shaderProgramCompileLog);
        std::cerr << "shader program linking failed: " << std::endl << this->shaderProgramCompileLog << std::endl;
        this->compileSuccess = 0;
        return 0;
    }
    else 
    {
        // if everything went smoothly...
        this->compileSuccess = 1;
        this->programID = shaderProgram;
        std::cout << "shader program compiled and linked successfully" << std::endl;

        // get and bind the location of shader attributes and uniforms
        // we can use these later without having to bother the gfx card since most glGetX() functions are very slow

        // basic vertex shader input
        this->vertexPositionLocation = glGetAttribLocation(this->programID, "vertexPosition");
        this->vertexNormalLocation = glGetAttribLocation(this->programID, "vertexNormal");
        this->vertexUvLocation = glGetAttribLocation(this->programID, "vertexCoordinate");
        this->attributeLocations.push_back({ Engine::ShaderProgram::ShaderAttribute::VERTEX_POSITION_LOCATION, glGetAttribLocation(this->programID, "vertexPosition") });

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
        this->volumeMapLocation = glGetUniformLocation(this->programID, "volumeMap");
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

unsigned int Engine::ShaderProgram::GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute attribute)
{
    switch (attribute)
    {
    case Engine::ShaderProgram::ShaderAttribute::MODEL_LOCATION:
        return this->modelTransformLocation;

    case Engine::ShaderProgram::ShaderAttribute::VIEW_LOCATION:
        return this->viewTransformLocation;

    case Engine::ShaderProgram::ShaderAttribute::PROJECTION_LOCATION:
        return this->projectionTransformLocation;

    case Engine::ShaderProgram::ShaderAttribute::BONE_TRANSFORMATIONS:
        return this->BoneTransformsLocation;

    case Engine::ShaderProgram::ShaderAttribute::VERTEX_POSITION_LOCATION:
        return this->vertexPositionLocation;

    case Engine::ShaderProgram::ShaderAttribute::VERTEX_NORMAL_LOCATION:
        return this->vertexNormalLocation;

    case Engine::ShaderProgram::ShaderAttribute::VERTEX_UV_LOCATION:
        return this->vertexUvLocation;

    case Engine::ShaderProgram::ShaderAttribute::VERTEX_BONE_ID_LOCATION:
        return this->vertexBoneIdLocation;

    case Engine::ShaderProgram::ShaderAttribute::VERTEX_BONE_WEIGHTS_LOCATION:
        return this->vertexBoneWeights;

    case Engine::ShaderProgram::ShaderAttribute::SHADER_PARAMETERS:
        return this->shaderFlagsLocation;

    case Engine::ShaderProgram::ShaderAttribute::DIFFUSE_MAP:
        return this->diffuseMapLocation;

    case Engine::ShaderProgram::ShaderAttribute::ROUGHNESS_MAP:
        return this->roughnessMapLocation;

    case Engine::ShaderProgram::ShaderAttribute::METALLIC_MAP:
        return this->metallicMapLocation;

    case Engine::ShaderProgram::ShaderAttribute::SPECULAR_MAP:
        return this->specularMapLocation;

    case Engine::ShaderProgram::ShaderAttribute::NORMAL_MAP:
        return this->normalMapLocation;

    case Engine::ShaderProgram::ShaderAttribute::ALPHA_MAP:
        return this->alphaMapLocation;

    case Engine::ShaderProgram::ShaderAttribute::CUBE_MAP:
        return this->cubeMapLocation;

    case Engine::ShaderProgram::ShaderAttribute::VOLUME_MAP:
        return this->volumeMapLocation;

    case Engine::ShaderProgram::ShaderAttribute::SHADOW_MAPS:
        return this->shadowMapsLocation;

    default:
        break;
    }
}

Engine::ShaderProgram::ShaderFlag& Engine::ShaderProgram::GetShaderFlags()
{
    return this->shaderFlags;
}

bool Engine::ShaderProgram::GetShaderFlag(Engine::ShaderProgram::ShaderFlag flag)
{
    return bool(unsigned int(this->shaderFlags) & unsigned int(flag));
}

unsigned int Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer buffer)
{
    switch (buffer)
    {
    case Engine::ShaderProgram::UniformBuffer::MVP_MATRICES:
        return Engine::ShaderProgram::mvpBlock;

    case Engine::ShaderProgram::UniformBuffer::POINT_LIGHTS:
        return Engine::ShaderProgram::pointLightsBLock;

    case Engine::ShaderProgram::UniformBuffer::SPOT_LIGHTS:
        return Engine::ShaderProgram::spotLightsBLock;

    case Engine::ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS:
        return Engine::ShaderProgram::directionalLightsBLock;

    case Engine::ShaderProgram::UniformBuffer::AMBIENT_LIGHTS:
        return Engine::ShaderProgram::ambientLightsBLock;

    case Engine::ShaderProgram::UniformBuffer::MATERIAL_PARAMETERS:
        return Engine::ShaderProgram::materialParametersBlock;

    default:
        break;
    }
}

void Engine::ShaderProgram::SetShaderFlag(Engine::ShaderProgram::ShaderFlag flag)
{
    this->shaderFlags = flag;

    glUniform1ui(this->shaderFlagsLocation, unsigned int(this->shaderFlags));
}

void Engine::ShaderProgram::Activate()
{
    Engine::ShaderProgram::currentShaderProgram = this;
    glUseProgram(this->programID);
}

Engine::ShaderProgram& Engine::ShaderProgram::GetCurrentShaderProgram()
{
    return *Engine::ShaderProgram::currentShaderProgram;
}

std::string Engine::ShaderProgram::GetLogData()
{
    std::string vs(this->vertexShader.GetCompileLog());
    std::string gs {};

    if (this->geometryShader) 
        gs = this->geometryShader->GetCompileLog();

    std::string fg(this->fragmentShader.GetCompileLog());
    std::string sp(this->shaderProgramCompileLog);

    std::string log = "vertex Shader log: \n" + vs;
    log += "\n geometry shader log: \n" + gs;
    log += "\n fragment Shader log: \n" + fg;
    log += "\n shader program log: \n" + sp;

    return log;
}

Engine::VertexShader& Engine::ShaderProgram::GetVertexShader()
{
    return this->vertexShader;
}

Engine::GeometryShader& Engine::ShaderProgram::GetGeometryShader()
{
    return *this->geometryShader;
}

Engine::FragmentShader& Engine::ShaderProgram::GetFragmentShader()
{
    return this->fragmentShader;
}

unsigned int Engine::ShaderProgram::GetProgramID()
{
    return this->programID;
}

Engine::Shader::Shader()
{
    *this->compileLog = {};
    this->compilationStatus = false;
    this->shader = {};
}

void Engine::Shader::AddSource(std::string filePath)
{
    // read shader data from the file
	std::string shaderCode;
    char temp = {};
	int success = false;
	int length = 0;

	std::ifstream reader(filePath);

	while (!reader.eof())
	{
		reader.read(&temp, 1);
        shaderCode += temp;
	}
	reader.close();
	const char* data = shaderCode.c_str();
	length = shaderCode.size();

	// shader compile and bind
    this->SetShaderType();
	glShaderSource(this->shader, 1, &data, NULL);
	glCompileShader(this->shader);
	glGetShaderiv(this->shader, GL_COMPILE_STATUS, &success);

    // check if everything went smoothly
	if (!success) {
		glGetShaderInfoLog(this->shader, 512, NULL, this->compileLog);
		std::cerr << filePath << typeid(*this).name() << " shader compilation failed: " << std::endl << this->compileLog << std::endl;
		std::cerr << typeid(*this).name() << ": " << std::endl << data << ", Length: " << length << std::endl;
        this->compilationStatus = false;
		delete data;
	}
    else {
        std::cout << filePath << typeid(*this).name() << " shader compiled successfully" << std::endl;
        this->compilationStatus = true;
    }
	
}

char* Engine::Shader::GetCompileLog()
{
    return this->compileLog;
}

unsigned int Engine::Shader::GetShader()
{
    return this->shader;
}

bool Engine::Shader::GetCompilatonStatus()
{
    return this->compilationStatus;
}

Engine::Shader::~Shader()
{
    glDeleteShader(this->shader);
}

void Engine::VertexShader::SetShaderType()
{
    this->shader = glCreateShader(GL_VERTEX_SHADER);
}

void Engine::GeometryShader::SetShaderType()
{
    this->shader = glCreateShader(GL_GEOMETRY_SHADER);
}

void Engine::FragmentShader::SetShaderType()
{
    this->shader = glCreateShader(GL_FRAGMENT_SHADER);
}

void Engine::ComputeShader::SetShaderType()
{
    this->shader = glCreateShader(GL_COMPUTE_SHADER);
}
