#include "Texture3D.h"

Engine::Texture3D::Texture3D()
{
    this->data = {};
    this->width = {};
    this->height = {};
    this->numChannels = {};
    this->textureID = {};
}

Engine::Texture3D::~Texture3D()
{
    // if (this->data != NULL) delete data;
}

Engine::Texture3D::Texture3D(std::string filePath)
{
    this->data = {};
    this->width = {};
    this->height = {};
    this->numChannels = {};
    
    this->Setup(filePath);
}

Engine::Texture3D::Texture3D(unsigned int width, unsigned int height, unsigned int depth, unsigned int numChannels, float* data)
{
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_3D, this->textureID);

    // figure out which texture format it is
    switch (numChannels) {
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

    glTexImage3D(GL_TEXTURE_3D, 0, numChannels, width, height, depth, 0, numChannels, GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_3D);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    this->width = width;
    this->height = height;
    this->depth = depth;
    this->numChannels = numChannels;
    *this->data = unsigned char(data);
}

void Engine::Texture3D::Setup(std::string filePath)
{
    
}

unsigned char* Engine::Texture3D::GetData()
{
    return this->data;
}

unsigned int Engine::Texture3D::GetWidth()
{
    return this->width;
}

unsigned int Engine::Texture3D::GetHeight()
{
    return this->height;
}

unsigned int Engine::Texture3D::GetDepth()
{
    return this->depth;
}

unsigned int Engine::Texture3D::GetNumChannels()
{
    return this->numChannels;
}

unsigned int Engine::Texture3D::GetTextureID()
{
    return this->textureID;
}

void Engine::Texture3D::SetWidth(unsigned int width)
{
    this->width = width;
}

void Engine::Texture3D::SetHeight(unsigned int height)
{
    this->height = height;
}

void Engine::Texture3D::SetDepth(unsigned int depth)
{
    this->depth = depth;
}

void Engine::Texture3D::SetNumChannels(unsigned int numChannels)
{
    this->numChannels = numChannels;
}

void Engine::Texture3D::SetData(unsigned char* data)
{
    this->data = data;
}

void Engine::Texture3D::SetTextureID(unsigned int id)
{
    this->textureID = id;
}