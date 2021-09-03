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

        // handles for Vertex Buffer
        // needs to be passed to OpenGL functions that are supposed to draw the mesh
        unsigned int GetVBO();
        unsigned int GetVAO();

        // cubemap vertices
        float* GetVertices();

        void Setup();

    private:
        Engine::Texture texture;
        Engine::Shader shader;

        // vertex  and element buffer objects
        unsigned int VAO;
        unsigned int VBO;

        std::unique_ptr<float*> vertices;

    };

}
#endif //  SKYBOX_H
