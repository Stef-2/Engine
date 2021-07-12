#ifndef SHADER_H
#define SHADER_H


#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "glfw3.h"

#include "fstream"
#include "sstream"
#include "string"
#include "iostream"
#include "memory"

namespace Engine
{

//OpenGL shader program wrapper class
//encapsulates vertex and fragment shaders
//as well as the shader program that binds them
class Shader
{
    public:
        Shader();
        Shader(const char* vertexShader, const char* fragmentShader);

        Shader(const Shader& other);
        Shader& operator=(const Shader& other);
        ~Shader();

        unsigned int GetVertexShader();
        unsigned int GetFragmentShader();
        unsigned int GetProgramID();
        std::string GetLogData();

        int SetVertexShader(const char* filePath);
        int SetFragmentShader(const char* filePath);

        int CompileProgram(); //needs to be run after every shader change
        void Run(); //needs to be run before every draw call that will make use of this shader

    private:
        int compileSuccess;
        char vsLog[512]; //vertex shader compile log
        char fsLog[512]; //fragment shader compile log
        char spLog[512]; //shader program compile log

        //handles for the shaders and program themselves
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int programID;

};

}

#endif // SHADER_H
