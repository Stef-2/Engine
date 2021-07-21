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

//texture loading and handling class
namespace Engine
{

    class Texture
    {
        public:
            Texture();
            Texture(const char* filePath);
            ~Texture();

            //loads the texture from a file
            //and binds it into an OpenGL texture object
            void Setup(const char* filePath);
            void SetupObj(std::string data);

            //this is the handle to the texture object
            //pass it to OpenGL before any rendering using this texture takes place
            unsigned int GetTextureID();
            
            //raw texture data extracted from the file
            unsigned char* GetData();

            int GetWidth();
            int GetHeight();
            int GetDepth();

        private:
            int width;
            int height;
            int depth;
            unsigned char* data;
            unsigned int textureID;
    };

}
#endif // TEXTURE_H
