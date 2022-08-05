#include "Texture.h"

Engine::Texture2D::Texture2D()
{
	this->data = {};
	this->width = {};
	this->height = {};
	this->numChannels = {};
	this->textureID = {};
}

Engine::Texture2D::~Texture2D()
{
	//if (this->data) delete data;
	/*
	if (this->textureID)
		glDeleteTextures(1, &this->textureID);*/
}

Engine::Texture2D::Texture2D(std::string filePath)
{
	this->data = {};
	this->width = {};
	this->height = {};
	this->numChannels = {};

	this->Setup(filePath);
}

Engine::Texture2D::Texture2D(std::string filePaths[6])
{
	this->data = {};
	this->width = {};
	this->height = {};
	this->numChannels = {};

	this->Setup(filePaths);
}

void Engine::Texture2D::Setup(std::string filePath)
{
	int width;
	int height;
	int nPixelComponents;

	unsigned int texture;
	
	// stbi tends to flip images for some reason
	stbi_set_flip_vertically_on_load(true);
	// load data
	this->data = stbi_load(filePath.c_str(), &width, &height, &nPixelComponents, 0);
	
	//check if its valid
	if (this->data) {

		GLenum numChannels;

		// figure out which texture format it is
		switch (nPixelComponents) {
		case 1:
			numChannels = GL_RED;
			break;
		case 2:
			numChannels = GL_RG;
			break;
		case 3:
			numChannels = GL_RGB;
			break;
		case 4:
			numChannels = GL_RGBA;
			break;
		default:
			numChannels = GL_RGBA;
			break;
		}

		//this->textureID = texture;
		this->width = width;
		this->height = height;
		this->numChannels = numChannels;

		this->GenerateTexture();

		stbi_image_free(this->data);
	}
	else {
		std::cerr << "Texture file at: " << filePath << " could not be loaded." << std::endl;
		this->data = 0;
		this->width = 0;
		this->height = 0;
		this->numChannels = 0;
	}
}

void Engine::Texture2D::Setup(std::string filePaths[6])
{
	int width;
	int height;
	int nPixelComponents;
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	// stbi tends to flip images for some reason
	stbi_set_flip_vertically_on_load(false);

	for (size_t i = 0; i < 6; i++)
	{
		// load data
		this->data = stbi_load(filePaths[i].c_str(), &width, &height, &nPixelComponents, 0);

		// check if its valid
		if (this->data) {

			GLenum numChannels;

			// figure out which texture format it is
			switch (nPixelComponents) {
			case 1:
				numChannels = GL_RED;
				break;
			case 2:
				numChannels = GL_RG;
				break;
			case 3:
				numChannels = GL_RGB;
				break;
			case 4:
				numChannels = GL_RGBA;
				break;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, numChannels, width, height, 0, numChannels, GL_UNSIGNED_BYTE, this->data);

			stbi_image_free(this->data);
		}
		else
		{
			std::cerr << "Texture file at: " << filePaths[i] << " could not be loaded." << std::endl;
			this->data = nullptr;
			this->width = 0;
			this->height = 0;
			this->numChannels = 0;
		}

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	this->textureID = texture;
	this->width = width;
	this->height = height;
	this->numChannels = nPixelComponents;
}

unsigned char* Engine::Texture2D::GetData()
{
	return this->data;
}

unsigned int Engine::Texture2D::GetWidth()
{
	return this->width;
}

unsigned int Engine::Texture2D::GetHeight()
{
	return this->height;
}

unsigned int Engine::Texture2D::GetNumChannels()
{
	return this->numChannels;
}

unsigned int Engine::Texture2D::GetTextureID()
{
	return this->textureID;
}

void Engine::Texture2D::SetWidth(unsigned int width)
{
	this->width = width;
}

void Engine::Texture2D::SetHeight(unsigned int height)
{
	this->height = height;
}

void Engine::Texture2D::SetNumChannels(unsigned int numChannels)
{
	this->numChannels = numChannels;
}

void Engine::Texture2D::GenerateTexture()
{
	glGenTextures(1, &this->textureID);

	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->numChannels, this->width, this->height, 0, this->numChannels, GL_UNSIGNED_BYTE, this->data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Engine::Texture3D::GenerateTexture()
{
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_3D, this->textureID);

	glTexImage3D(GL_TEXTURE_3D, 0, this->numChannels, this->width, this->height, this->depth, 0, this->numChannels, GL_FLOAT, this->data);
	glGenerateMipmap(GL_TEXTURE_3D);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Engine::Texture2D::SetData(unsigned char* data)
{
	this->data = data;
}

void Engine::Texture2D::SetTextureID(unsigned int id)
{
	this->textureID = id;
}

void Engine::Texture2D::Activate()
{
	glBindTexture(GLenum(Engine::Texture::TextureType::TEXTURE2D), this->textureID);
}

Engine::Texture3D::Texture3D(std::string filePath)
{
	this->data = {};
	this->width = {};
	this->height = {};
	this->depth = {};
	this->numChannels = {};

	this->Setup(filePath);
}

void Engine::Texture3D::Activate()
{
	glBindTexture(GLenum(Engine::Texture::TextureType::TEXTURE3D), this->textureID);
}

unsigned int Engine::Texture3D::GetDepth()
{
	return this->depth;
}

void Engine::Texture3D::SetDepth(unsigned int depth)
{
	this->depth = depth;
}

Engine::Image2D::Image2D(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	// hack, tofix
	this->numChannels = GL_RGBA;

	this->textureID = {};
	this->data = {};

	this->GenerateTexture();
}

void Engine::Image2D::Activate()
{
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glBindImageTexture(0, this->textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void Engine::Image2D::GenerateTexture()
{
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, this->numChannels, GL_FLOAT, 0);

	glBindImageTexture(0, this->textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

Engine::Image2D::Image2D(unsigned int width, unsigned int height, unsigned short numChannels)
{
	this->width = width;
	this->height = height;

	// figure out which texture format it is
	switch (numChannels) {
	case 1:
		this->numChannels = GL_RED;
		break;
	case 2:
		this->numChannels = GL_RG;
		break;
	case 3:
		this->numChannels = GL_RGB;
		break;
	case 4:
		this->numChannels = GL_RGBA;
		break;
	}

	this->textureID = {};
	this->data = {};

	this->GenerateTexture();
}

Engine::Image3D::Image3D(unsigned int width, unsigned int height, unsigned int depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	// hack, tofix
	this->numChannels = GL_RGBA;

	this->textureID = {};
	this->data = {};

	this->GenerateTexture();
}

void Engine::Image3D::Activate()
{
	glBindTexture(GL_TEXTURE_3D, this->textureID);
	glBindImageTexture(0, this->textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void Engine::Image3D::GenerateTexture()
{
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_3D, this->textureID);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, this->width, this->height, this->depth, 0, this->numChannels, GL_FLOAT, 0);

	glBindImageTexture(0, this->textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

Engine::Image3D::Image3D(unsigned int width, unsigned int height, unsigned int depth, unsigned short numChannels)
{
	this->width = width;
	this->height = height;
	this->depth = depth;

	// figure out which texture format it is
	switch (numChannels) {
	case 1:
		this->numChannels = GL_RED;
		break;
	case 2:
		this->numChannels = GL_RG;
		break;
	case 3:
		this->numChannels = GL_RGB;
		break;
	case 4:
		this->numChannels = GL_RGBA;
		break;
	}

	this->textureID = {};
	this->data = {};

	this->GenerateTexture();
}
