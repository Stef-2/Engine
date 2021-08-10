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

            //enumerator for the different shader attribute we may wish to retrive
            typedef enum class ShaderAttribute {
                MODEL_LOCATION,
                VIEW_LOCATION,
                PROJECTION_LOCATION,
                VERTEX_POSITION_LOCATION,
                VERTEX_NORMAL_LOCATION,
                VERTEX_UV_LOCATION
            };

            Shader();
            Shader(const char* vertexShader, const char* fragmentShader);

            //Shader(const Shader& other);
            //Shader& operator=(const Shader& other);
            //~Shader();

            //glGetCurrentProgram() is very slow, just like all functions that ask the gfx card for data
            //so we're storing it ourselves in case we need it
            static Shader* GetCurrentShader();

            unsigned int GetVertexShader();
            unsigned int GetFragmentShader();
            unsigned int GetProgramID();

            unsigned int GetAttributeLocation(ShaderAttribute attribute);

            std::string GetLogData();

            //load vertex and fragment shaders from files
            int SetVertexShader(const char* filePath);
            int SetFragmentShader(const char* filePath);

            //compiles vertex and fragment shaders into a program and binds it
            //needs to be run after every shader change
            int CompileProgram();

            //activates this shader program
            //needs to be run before every draw call that will make use of this shader
            void Activate();

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

            //shader attribute locations
            unsigned int vertexPositionLocation;
            unsigned int vertexNormalLocation;
            unsigned int vertexUvLocation;
            unsigned int modelTransformLocation;
            unsigned int viewTransformLocation;
            unsigned int projectionTransformLocation;

            static Shader* currentShader;
    };

}

#endif // SHADER_H
