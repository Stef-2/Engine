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

        //Shader(const Shader& other);
        //Shader& operator=(const Shader& other);
        //~Shader();

        unsigned int GetVertexShader();
        unsigned int GetFragmentShader();
        unsigned int GetProgramID();
        std::string GetLogData();

        //load vertex and fragment shaders from files
        int SetVertexShader(const char* filePath);
        int SetFragmentShader(const char* filePath);

        //compiles vertex and fragment shaders into a program and binds it
        //needs to be run after every shader change
        int CompileProgram();

        //activates this shader program
        //needs to be run before every draw call that will make use of this shader
        void Run();

    private:
        //will be set to 1 if shader compilation succeeds, 0 otherwise
        int compileSuccess;
        //vertex shader compile log
        char vsLog[512];
        //fragment shader compile log
        char fsLog[512];
        //shader program compile log
        char spLog[512];

        //handles for the shaders and program themselves
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int programID;
};

}

#endif // SHADER_H
