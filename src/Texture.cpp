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
    }
    else {
        std::cerr << "Texture file at: " << filePath << " could not be loaded." << std::endl;
        this->data = 0;
        this->width = 0;
        this->height = 0;
        this->depth = 0;
    }

    stbi_image_free(this->data);
}

void Engine::Texture::SetupObj(std::string data)
{
    int width = 512, height = 512, depth = 16;
    unsigned int texture;


    if (!data.empty()) {

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.c_str());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        this->width = width;
        this->height = height;
        this->depth = depth;
        this->textureID = texture;
    }
    else {
        std::cerr << "Obj texture file could not be loaded." << std::endl;
        this->data = 0;
        this->width = 0;
        this->height = 0;
        this->depth = 0;
    }
}

unsigned char* Engine::Texture::GetData()
{
    return this->data;
}

int Engine::Texture::GetWidth()
{
    return this->width;
}

int Engine::Texture::GetHeight()
{
    return this->height;
}

int Engine::Texture::GetDepth()
{
    return this->depth;
}

unsigned int Engine::Texture::GetTextureID()
{
    return this->textureID;
}
