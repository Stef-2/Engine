#include "Texture.h"

Engine::Texture::~Texture()
{
    if (this->data != NULL) delete data;
}

Engine::Texture::Texture(const char* filePath)
{
    int width, height, depth;
    unsigned int texture;

    this->data = stbi_load(filePath, &width, &height, &depth, 0);

    if (this->data) {

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

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

unsigned char Engine::Texture::GetData()
{
    return *this->data;
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
