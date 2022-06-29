#include "Texture2D.h"

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
    // if (this->data != NULL) delete data;
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
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, numChannels, width, height, 0, numChannels, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        this->textureID = texture;
        this->width = width;
        this->height = height;
        this->numChannels = numChannels;

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
            this->data = 0;
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

void Engine::Texture2D::SetData(unsigned char* data)
{
    this->data = data;
}

void Engine::Texture2D::SetTextureID(unsigned int id)
{
    this->textureID = id;
}