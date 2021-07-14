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
//the constructor binds it into an openGL texture
//maybe that functionality should be decoupled
namespace Engine
{

    class Texture
    {
        public:
            Texture();
            Texture(const char* filePath);
            ~Texture();

            void Setup(const char* filePath);
            void SetupObj(std::string data);
            unsigned char* GetData();
            int GetWidth();
            int GetHeight();
            int GetDepth();
            unsigned int GetTextureID();

        private:
            int width;
            int height;
            int depth;
            unsigned char* data;
            unsigned int textureID;
    };

}
#endif // TEXTURE_H
