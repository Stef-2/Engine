#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

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

    class Texture
    {
    public:
        Texture();

        // regular textures
        Texture(std::string filePath);
        Texture(std::string filePaths[6]);

        ~Texture();

        // loads the texture from a file
        // and binds it into an OpenGL texture object
        void Setup(std::string filePath);

        // loads 6 texture files for skyboxes (Cubemaps)
        // order is left, right, top, bottom, front, back
        void Setup(std::string filePaths[6]);

        // this is the handle to the texture object
        // pass it to OpenGL before any rendering using this texture takes place
        unsigned int GetTextureID();
        void SetTextureID(unsigned int id);
            
        // raw texture data extracted from the file
        unsigned char* GetData();
        void SetData(unsigned char* data);

        unsigned int GetWidth();
        unsigned int GetHeight();
        unsigned int GetNumChannels();

        void SetWidth(unsigned int width);
        void SetHeight(unsigned int height);
        void SetNumChannels(unsigned int numChannels);

    private:
        unsigned int width;
        unsigned int height;
        unsigned int numChannels;
        unsigned char* data;
        unsigned int textureID;
    };

}
#endif //  TEXTURE_H
