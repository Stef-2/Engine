#include "Shader.h"

std::map<Engine::ShaderProgram::UniformBuffer, unsigned int> Engine::ShaderProgram::UniformBufferLocations = {};
Engine::ShaderProgram* Engine::ShaderProgram::currentShaderProgram = {};

std::string Engine::ShaderProgram::DebugOutput()
{
	struct OutputData
	{
		glm::mat4 mats[8];
		glm::vec4 vecs[8];
		float floats[8];
	} shaderOutput = {};

	glGetNamedBufferSubData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::DEBUG),
		0, sizeof(OutputData), &shaderOutput);

	std::string output{ "=== Shader debug output: \n matrices: \n"};

	for (auto& i : shaderOutput.mats)
		if (i != glm::mat4(0))
			output += glm::to_string(i) + '\n';

	output += "vectors: \n";

	for (auto& i : shaderOutput.vecs)
		if (i != glm::vec4(0))
			output += glm::to_string(i) + '\n';

	output += "floats: \n";

	for (auto& i : shaderOutput.floats)
		output += std::to_string(i) + " ";

	output += '\n';

	return output;
}

Engine::ShaderProgram::ShaderProgram()
{
	this->compileSuccess = false;

	this->programID = 0;
	this->shaderFlags = Engine::ShaderProgram::ShaderFlag(0u);

}

Engine::ShaderProgram::ShaderProgram(Engine::ComputeShader& shader)
{
	this->compileSuccess = false;

	this->programID = 0;
	this->shaderFlags = Engine::ShaderProgram::ShaderFlag(0u);

	this->SetComputeShader(shader);
	
	this->compileSuccess = this->CompileProgram();
}

Engine::ShaderProgram::ShaderProgram(std::string computerShader)
{
	this->compileSuccess = false;
	this->programID = 0;
	this->shaderFlags = Engine::ShaderProgram::ShaderFlag(0u);

	this->SetComputeShader(computerShader);

	this->compileSuccess = this->CompileProgram();
}

Engine::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->programID);

	if (Engine::ShaderProgram::currentShaderProgram == this)
		Engine::ShaderProgram::currentShaderProgram = nullptr;
}

Engine::ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader)
{
	this->compileSuccess = false;
	this->programID = 0;
	this->shaderFlags = Engine::ShaderProgram::ShaderFlag(0u);

	this->SetVertexShader(vertexShader);
	this->SetFragmentShader(fragmentShader);

	this->compileSuccess = this->CompileProgram();
}

Engine::ShaderProgram::ShaderProgram(std::string vertexShader, std::string geometryShader, std::string fragmentShader)
{
	this->compileSuccess = false;
	this->programID = 0;
	this->shaderFlags = Engine::ShaderProgram::ShaderFlag(0u);

	this->SetVertexShader(vertexShader);
	this->SetGeometryShader(geometryShader);
	this->SetFragmentShader(fragmentShader);

	this->compileSuccess = this->CompileProgram();
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

void Engine::ShaderProgram::SetComputeShader(std::string filePath)
{
	this->computeShader = std::make_shared<Engine::ComputeShader>();
	this->computeShader->AddSource(filePath);
}

void Engine::ShaderProgram::SetVertexShader(Engine::VertexShader& shader)
{
	this->vertexShader = std::make_shared<Engine::VertexShader>(shader);
}

void Engine::ShaderProgram::SetComputeShader(Engine::ComputeShader& shader)
{
	this->computeShader = std::make_shared<Engine::ComputeShader>(shader);
}

void Engine::ShaderProgram::SetGeometryShader(Engine::GeometryShader& shader)
{
	this->geometryShader = std::make_shared<Engine::GeometryShader>(shader);
}

void Engine::ShaderProgram::SetFragmentShader(Engine::FragmentShader& shader)
{
	this->fragmentShader = std::make_shared<Engine::FragmentShader>(shader);
}

bool Engine::ShaderProgram::CompileProgram()
{
	// for compute shader programs, only the compute shader is needed
	if (!this->computeShader.Get())
	{
		// for regular rendering pipeline, vertex and fragment shaders are needed
		if (this->vertexShader.Get() && !this->vertexShader->GetShader()) {
			std::cerr << "unable to compile shader program, vertex shader is missing" << std::endl;
			this->compileSuccess = false;
			this->programID = 0;
			return false;
		}
		if (this->fragmentShader.Get() && !this->fragmentShader->GetShader()) {
			std::cerr << "unable to compile shader program, fragment shader is missing" << std::endl;
			this->compileSuccess = false;
			this->programID = 0;
			return false;
		}
	}
	else if (!this->computeShader->GetShader()) {
		std::cerr << "unable to compile compute shader program, shader is missing" << std::endl;
		this->compileSuccess = false;
		this->programID = 0;
		return false;
		}

	int success;
	char compileLog[512];

	// generate the main shader program
	this->programID = glCreateProgram();

	// attach shaders
	if (this->vertexShader.Get())
		glAttachShader(this->programID, this->vertexShader->GetShader());

	if (this->geometryShader.Get())
		glAttachShader(this->programID, this->geometryShader->GetShader());

	if (this->fragmentShader.Get())
		glAttachShader(this->programID, this->fragmentShader->GetShader());

	if (this->computeShader.Get())
		glAttachShader(this->programID, this->computeShader->GetShader());

	// link and compile
	glLinkProgram(this->programID);
	glGetProgramiv(this->programID, GL_LINK_STATUS, &success);

	// delete vertex and fragment shaders once we don't need them anymore
	//glDeleteShader(this->vertexShader->GetShader());
	//glDeleteShader(this->fragmentShader->GetShader());

	// check for and print any errors we may have had
	if(!success) {
		
		glGetProgramInfoLog(this->programID, 512, NULL, compileLog);
		std::cerr << "shader program linking failed: " << std::endl << compileLog << std::endl;
		return false;
	}
	else 
	{
		// if everything went smoothly...
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
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::IMAGE2D).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::IMAGE2D, location });
		location = glGetUniformLocation(this->programID, ShaderProgram::attributeNames.at(ShaderAttribute::IMAGE3D).c_str());
		if (location >= 0) this->attributeLocations.insert({ ShaderAttribute::IMAGE3D, location });

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

		// ambient lights
		if (glGetProgramResourceIndex(this->programID, GL_SHADER_STORAGE_BLOCK, ShaderProgram::uniformBufferNames.at(ShaderProgram::UniformBuffer::DEBUG).c_str()) != GL_INVALID_INDEX
			&& !ShaderProgram::UniformBufferLocations.contains(ShaderProgram::UniformBuffer::DEBUG))
		{
			// add the pair to the map
			ShaderProgram::UniformBufferLocations.insert
			({ ShaderProgram::UniformBuffer::DEBUG, glGetUniformBlockIndex(this->programID,
				ShaderProgram::uniformBufferNames.at(UniformBuffer::DEBUG).c_str()) });

			// struct that matches the output data block in the shaders
			// to be used as sizeof() and offsetof() reference in data alignment and size matching
			struct OutputData
			{
				glm::mat4 mats[8];
				glm::vec4 vecs[8];
				float floats[8];
			};

			glGenBuffers(1, &ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::DEBUG));
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 69, ShaderProgram::UniformBufferLocations.at(ShaderProgram::UniformBuffer::DEBUG));
			glBufferData(GL_UNIFORM_BUFFER, sizeof(OutputData), NULL, GL_DYNAMIC_DRAW);
			glNamedBufferData(Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::DEBUG),
				sizeof(OutputData), 0, GL_DYNAMIC_DRAW);
		}

		return true;
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

Engine::ComputeShader& Engine::ShaderProgram::GetComputeShader()
{
	return *this->computeShader;
}

unsigned int Engine::ShaderProgram::GetProgramID()
{
	return this->programID;
}

Engine::Shader::Shader(std::string filePath)
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
		//delete data;
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
	this->shader = glCreateShader(GLenum(Engine::Shader::ShaderType::VERTEX_SHADER));
}

void Engine::GeometryShader::SetShaderType()
{
	this->shader = glCreateShader(GLenum(Engine::Shader::ShaderType::GEOMETRY_SHADER));
}

void Engine::FragmentShader::SetShaderType()
{
	this->shader = glCreateShader(GLenum(Engine::Shader::ShaderType::FRAGMENT_SHADER));
}

void Engine::ComputeShader::SetShaderType()
{
	this->shader = glCreateShader(GLenum(Engine::Shader::ShaderType::COMPUTE_SHADER));
}
