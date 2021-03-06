#include "Shader.h"

std::map<Engine::ShaderProgram::UniformBuffer, unsigned int> Engine::ShaderProgram::UniformBufferLocations = {};
Engine::ShaderProgram* Engine::ShaderProgram::currentShaderProgram = {};

Engine::ShaderProgram::ShaderProgram()
{
    this->compileSuccess = 0;

    this->programID = 0;
    this->shaderFlags = Engine::ShaderProgram::ShaderFlag(0u);

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

	this->SetVertexShader(vertexShader);
	this->SetGeometryShader(geometryShader);
	this->SetFragmentShader(fragmentShader);

    this->compileSuccess = this->CompileProgram();
}

void Engine::ShaderProgram::SetVertexShader(Engine::VertexShader& shader)
{
    this->vertexShader = std::make_shared<Engine::VertexShader>(shader);
}

void Engine::ShaderProgram::SetVertexShader(std::string filePath)
{
    this->vertexShader = std::make_shared<Engine::VertexShader>();
    this->vertexShader->AddSource(filePath);
}

void Engine::ShaderProgram::SetGeometryShader(std::string filePath)
{
    this->geometryShader = std::make_shared<Engine::GeometryShader>();
    this->geometryShader->AddSource(filePath);
}

void Engine::ShaderProgram::SetFragmentShader(std::string filePath)
{
    this->fragmentShader = std::make_shared<Engine::FragmentShader>();
    this->fragmentShader->AddSource(filePath);
}

void Engine::ShaderProgram::SetGeometryShader(Engine::GeometryShader& shader)
{
    *this->geometryShader = shader;
}

void Engine::ShaderProgram::SetFragmentShader(Engine::FragmentShader& shader)
{
    this->fragmentShader = std::make_shared<Engine::FragmentShader>(shader);
}

int Engine::ShaderProgram::CompileProgram()
{
    // check if both the required shaders are present
    if(this->vertexShader && !this->vertexShader->GetShader()) {
        std::cerr << "unable to compile shader program, vertex shader is missing" << std::endl;
        this->compileSuccess = 0;
        this->programID = 0;
        return 0;
    }
    if(this->fragmentShader && !this->fragmentShader->GetShader()) {
        std::cerr << "unable to compile shader program, fragment shader is missing" << std::endl;
        this->compileSuccess = 0;
        this->programID = 0;
        return 0;
    }
    
	int success;
	char compileLog[512];

    // main shader program compile and link
    this->programID = glCreateProgram();

    glAttachShader(this->programID, this->vertexShader->GetShader());

    if (this->geometryShader)
        glAttachShader(this->programID, this->geometryShader->GetShader());

    glAttachShader(this->programID, this->fragmentShader->GetShader());

    glLinkProgram(this->programID);
    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);

    // delete vertex and fragment shaders once we don't need them anymore
    glDeleteShader(this->vertexShader->GetShader());
    glDeleteShader(this->fragmentShader->GetShader());

    // check for and print any errors we may have had
    if(!success) {
        
        glGetProgramInfoLog(this->programID, 512, NULL, compileLog);
        std::cerr << "shader program linking failed: " << std::endl << compileLog << std::endl;
        this->compileSuccess = 0;
        return 0;
    }
    else 
    {
        // if everything went smoothly...
        this->compileSuccess = 1;
        std::cout << "shader program compiled and linked successfully" << std::endl;

        // get and bind the location of shader attributes and uniforms
        // we can use these later without having to bother the gfx card since most glGetX() functions are very slow
        volatile int location = 0u;

        // basic vertex shader input
        location = glGetAttribLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::VERTEX_POSITION_LOCATION).c_str());
        if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::VERTEX_POSITION_LOCATION, location });
		location = glGetAttribLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::VERTEX_NORMAL_LOCATION).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::VERTEX_NORMAL_LOCATION, location });
		location = glGetAttribLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::VERTEX_UV_LOCATION).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::VERTEX_UV_LOCATION, location });

        // basic MVP matrices with separate input
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::MODEL_LOCATION).c_str());
        if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::MODEL_LOCATION, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::VIEW_LOCATION).c_str());
        if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::VIEW_LOCATION, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::PROJECTION_LOCATION).c_str());
        if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::PROJECTION_LOCATION, location });

        // extended vertex shader inputs for animated meshes
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::BONE_TRANSFORMATIONS).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::BONE_TRANSFORMATIONS, location });
		location = glGetAttribLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::VERTEX_BONE_ID_LOCATION).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::VERTEX_BONE_ID_LOCATION, location });
		location = glGetAttribLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::VERTEX_BONE_WEIGHTS_LOCATION).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::VERTEX_BONE_WEIGHTS_LOCATION, location });

        // shader flags for uber shader
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::SHADER_PARAMETERS).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::SHADER_PARAMETERS, location });

        // uniform samplers for material textures
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::DIFFUSE_MAP).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::DIFFUSE_MAP, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::ROUGHNESS_MAP).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::ROUGHNESS_MAP, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::METALLIC_MAP).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::METALLIC_MAP, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::SPECULAR_MAP).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::SPECULAR_MAP, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::NORMAL_MAP).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::NORMAL_MAP, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::ALPHA_MAP).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::ALPHA_MAP, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::CUBE_MAP).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::CUBE_MAP, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::VOLUME_MAP).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::VOLUME_MAP, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::SHADOW_MAPS).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::SHADOW_MAPS, location });

        // generate and bind uniform buffer objects and shader storage buffer objects if they're present in the shader(s)
        // this initialization needs to happen only once for each one of these as they are global in GPU memory
        
        // check if the shader contains one of these uniform blocks and that our map of uniform vars - values doesn't already contain such a pair

        // model-view-projection (MVP) matrices
        if (glGetUniformBlockIndex(this->programID, ShaderProgram::uniformBufferNames.at(ShaderProgram::UniformBuffer::MVP_MATRICES).c_str()) != GL_INVALID_INDEX
            && !ShaderProgram::UniformBufferLocations.contains(ShaderProgram::UniformBuffer::MVP_MATRICES))
        {
            // add the pair to the map
            ShaderProgram::UniformBufferLocations.insert
            ({ShaderProgram::UniformBuffer::MVP_MATRICES, glGetUniformBlockIndex(this->programID,
                ShaderProgram::uniformBufferNames.at(UniformBuffer::MVP_MATRICES).c_str())});

            glGenBuffers(1, &ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::MVP_MATRICES));
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::MVP_MATRICES));
            glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
        }

        // material parameters
		if (glGetUniformBlockIndex(this->programID, ShaderProgram::uniformBufferNames.at(ShaderProgram::UniformBuffer::MATERIAL_PARAMETERS).c_str()) != GL_INVALID_INDEX
			&& !ShaderProgram::UniformBufferLocations.contains(ShaderProgram::UniformBuffer::MATERIAL_PARAMETERS))
        {
			// add the pair to the map
			ShaderProgram::UniformBufferLocations.insert
			({ ShaderProgram::UniformBuffer::MATERIAL_PARAMETERS, glGetUniformBlockIndex(this->programID,
				ShaderProgram::uniformBufferNames.at(UniformBuffer::MATERIAL_PARAMETERS).c_str()) });

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

            glGenBuffers(1, &ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::MATERIAL_PARAMETERS));
            glBindBufferBase(GL_UNIFORM_BUFFER, 5, ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::MATERIAL_PARAMETERS));
            glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(MaterialParametersReference), NULL, GL_DYNAMIC_DRAW);
        }

        // point lights
        if (glGetProgramResourceIndex(this->programID, GL_SHADER_STORAGE_BLOCK, ShaderProgram::uniformBufferNames.at(ShaderProgram::UniformBuffer::POINT_LIGHTS).c_str()) != GL_INVALID_INDEX
            && !ShaderProgram::UniformBufferLocations.contains(ShaderProgram::UniformBuffer::POINT_LIGHTS))
        {
			// add the pair to the map
			ShaderProgram::UniformBufferLocations.insert
			({ ShaderProgram::UniformBuffer::POINT_LIGHTS, glGetUniformBlockIndex(this->programID,
				ShaderProgram::uniformBufferNames.at(UniformBuffer::POINT_LIGHTS).c_str()) });

            glGenBuffers(1, &ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::POINT_LIGHTS));
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::POINT_LIGHTS));
        }
        
        // spot lights
		if (glGetProgramResourceIndex(this->programID, GL_SHADER_STORAGE_BLOCK, ShaderProgram::uniformBufferNames.at(ShaderProgram::UniformBuffer::SPOT_LIGHTS).c_str()) != GL_INVALID_INDEX
			&& !ShaderProgram::UniformBufferLocations.contains(ShaderProgram::UniformBuffer::SPOT_LIGHTS))
        {
			// add the pair to the map
			ShaderProgram::UniformBufferLocations.insert
			({ ShaderProgram::UniformBuffer::SPOT_LIGHTS, glGetUniformBlockIndex(this->programID,
				ShaderProgram::uniformBufferNames.at(UniformBuffer::SPOT_LIGHTS).c_str()) });

            glGenBuffers(1, &ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::SPOT_LIGHTS));
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::SPOT_LIGHTS));
        }

        // directional lights
		if (glGetProgramResourceIndex(this->programID, GL_SHADER_STORAGE_BLOCK, ShaderProgram::uniformBufferNames.at(ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS).c_str()) != GL_INVALID_INDEX
			&& !ShaderProgram::UniformBufferLocations.contains(ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS))
        {
			// add the pair to the map
			ShaderProgram::UniformBufferLocations.insert
			({ ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS, glGetUniformBlockIndex(this->programID,
				ShaderProgram::uniformBufferNames.at(UniformBuffer::DIRECTIONAL_LIGHTS).c_str()) });

            glGenBuffers(1, &ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS));
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::DIRECTIONAL_LIGHTS));
        }

        // ambient lights
		if (glGetProgramResourceIndex(this->programID, GL_SHADER_STORAGE_BLOCK, ShaderProgram::uniformBufferNames.at(ShaderProgram::UniformBuffer::AMBIENT_LIGHTS).c_str()) != GL_INVALID_INDEX
			&& !ShaderProgram::UniformBufferLocations.contains(ShaderProgram::UniformBuffer::AMBIENT_LIGHTS))
        {
			// add the pair to the map
			ShaderProgram::UniformBufferLocations.insert
			({ ShaderProgram::UniformBuffer::AMBIENT_LIGHTS, glGetUniformBlockIndex(this->programID,
				ShaderProgram::uniformBufferNames.at(UniformBuffer::AMBIENT_LIGHTS).c_str()) });

            glGenBuffers(1, &ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::AMBIENT_LIGHTS));
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::AMBIENT_LIGHTS));
        }

        return this->programID;
    }
}

unsigned int Engine::ShaderProgram::GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute attribute)
{
    if (this->attributeLocations.contains(attribute))
        return this->attributeLocations.at(attribute);
    else
        return 0;
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
    if (Engine::ShaderProgram::UniformBufferLocations.contains(buffer))
        return Engine::ShaderProgram::UniformBufferLocations.at(buffer);
    else
        return 0;
}

void Engine::ShaderProgram::SetShaderFlag(Engine::ShaderProgram::ShaderFlag flag)
{
    if (this->attributeLocations.contains(ShaderProgram::ShaderAttribute::SHADER_PARAMETERS))
    {
        this->shaderFlags = flag;
        glUniform1ui(this->ShaderProgram::attributeLocations.at(ShaderProgram::ShaderAttribute::SHADER_PARAMETERS), unsigned int(flag));
    }
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

Engine::VertexShader& Engine::ShaderProgram::GetVertexShader()
{
    return *this->vertexShader;
}

Engine::GeometryShader& Engine::ShaderProgram::GetGeometryShader()
{
    return *this->geometryShader;
}

Engine::FragmentShader& Engine::ShaderProgram::GetFragmentShader()
{
    return *this->fragmentShader;
}

unsigned int Engine::ShaderProgram::GetProgramID()
{
    return this->programID;
}

Engine::Shader::Shader()
{
    this->compilationStatus = false;
    this->shader = {};
}

void Engine::Shader::AddSource(std::string filePath)
{
    // read shader data from the file
	std::string shaderCode;
    char compileLog[512];
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
		glGetShaderInfoLog(this->shader, 512, NULL, compileLog);
		std::cerr << filePath << " " << typeid(*this).name() << " shader compilation failed: " << std::endl << compileLog << std::endl;
		std::cerr << typeid(*this).name() << ": " << std::endl << data << ", Length: " << length << std::endl;
        this->compilationStatus = false;
		delete data;
	}
    else {
        std::cout << filePath << " " << typeid(*this).name() << " shader compiled successfully" << std::endl;
        this->compilationStatus = true;
    }
	
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
