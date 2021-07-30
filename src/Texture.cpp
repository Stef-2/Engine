#include "Texture.h"

Engine::Texture::Texture()
{
    this->data = {};
    this->width = {};
    this->height = {};
    this->depth = {};
    this->textureID = {};
}

Engine::Texture::~Texture()
{
    //if (this->data != NULL) delete data;
}

Engine::Texture::Texture(const char* filePath)
{
    this->data = {};
    this->width = {};
    this->height = {};
    this->depth = {};

    this->Setup(filePath);
}

Engine::Texture::Texture(const char* filePaths[6])
{
    this->data = {};
    this->width = {};
    this->height = {};
    this->depth = {};

    this->Setup(filePaths);
}

void Engine::Texture::Setup(const char* filePath)
{
    int width, height, depth;
    unsigned int texture;

    stbi_set_flip_vertically_on_load(true);
    this->data = stbi_load(filePath, &width, &height, &depth, 0);

    if (this->data) {

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        this->textureID = texture;
        this->width = width;
        this->height = height;
        this->depth = depth;

        stbi_image_free(this->data);
    }
    else {
        std::cerr << "Texture file at: " << filePath << " could not be loaded." << std::endl;
        this->data = 0;
        this->width = 0;
        this->height = 0;
        this->depth = 0;
    }
}

void Engine::Texture::Setup(const char* filePaths[6])
{
    int width, height, depth;
    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    stbi_set_flip_vertically_on_load(false);

    for (size_t i = 0; i < 6; i++)
    {
        this->data = stbi_load(filePaths[i], &width, &height, &depth, 0);

        if (this->data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data);

            stbi_image_free(this->data);
        }
        else
        {
            std::cerr << "Texture file at: " << filePaths[i] << " could not be loaded." << std::endl;
            this->data = 0;
            this->width = 0;
            this->height = 0;
            this->depth = 0;
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
    this->depth = depth;
}

unsigned char* Engine::Texture::GetData()
{
    return this->data;
}

unsigned int Engine::Texture::GetWidth()
{
    return this->width;
}

unsigned int Engine::Texture::GetHeight()
{
    return this->height;
}

unsigned int Engine::Texture::GetDepth()
{
    return this->depth;
}

unsigned int Engine::Texture::GetTextureID()
{
    return this->textureID;
}

void Engine::Texture::SetWidth(unsigned int width)
{
    this->width = width;
}

void Engine::Texture::SetHeight(unsigned int height)
{
    this->height = height;
}

void Engine::Texture::SetDepth(unsigned int depth)
{
    this->depth = depth;
}

void Engine::Texture::SetData(unsigned char* data)
{
    this->data = data;
}

void Engine::Texture::SetTextureID(unsigned int id)
{
    this->textureID = id;
}