#ifndef SKYBOX_H
#define SKYBOX_H

#include "Object.h"
#include "Texture.h"
#include "Shader.h"

#include "fstream"
#include "string"

namespace Engine
{

    class Skybox : public Engine::Object
    {
        using Engine::Object::Object;

    public:
        Skybox();

        void SetShader(const Engine::Shader& shader);
        void SetTexture(const Engine::Texture& texture);

        Engine::Shader* GetShader();
        Engine::Texture* GetTexture();

        //handles for Vertex Buffer
        //needs to be passed to OpenGL functions that are supposed to draw the mesh
        unsigned int GetVBO();
        float* GetVertices();

        void Setup();

    private:
        Engine::Texture texture;
        Engine::Shader shader;

        //vertex buffer object
        unsigned int VBO;

        float* vertices;

    };

}
#endif // SKYBOX_H
