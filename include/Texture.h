#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "stb_image.h"
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
        Texture() = delete;
        Texture(const char* filePath);
        ~Texture();

        unsigned char GetData();
        int GetWidth();
        int GetHeight();
        int GetDepth();

    private:
        int width;
        int height;
        int depth;
        unsigned char* data;
};

}
#endif // TEXTURE_H
