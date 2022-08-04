#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "Shared.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "glfw3.h"
#include "fstream"
#include "iostream"

// texture loading and handling class
namespace Engine
{

    class Texture3D
    {
    public:
        Texture3D();

        // regular textures from a file
        Texture3D(std::string filePath);
        // generate a 3D texture by passing all the data and params manually
        Texture3D(unsigned int width, unsigned int height, unsigned int depth, unsigned int numChannels, float* data);

        ~Texture3D();

        // loads the texture from a file
        // and binds it into an OpenGL texture object
        void Setup(std::string filePath);

        // this is the handle to the texture object
        // pass it to OpenGL before any rendering using this texture
        unsigned int GetTextureID();
        void SetTextureID(unsigned int id);

        // raw texture data extracted from the file
        unsigned char* GetData();
        void SetData(unsigned char* data);
        
        unsigned int GetWidth();
        unsigned int GetHeight();
        unsigned int GetDepth();
        unsigned int GetNumChannels();

        void SetWidth(unsigned int width);
        void SetHeight(unsigned int height);
        void SetDepth(unsigned int depth);
        void SetNumChannels(unsigned int numChannels);

    private:
        unsigned int width;
        unsigned int height;
        unsigned int depth;

        unsigned int numChannels;
        unsigned char* data;
        unsigned int textureID;
    };

    // typedef for shared textures
    using SharedTexture3D = Engine::Shared<Texture3D>;
}
#endif //  TEXTURE3D_H
