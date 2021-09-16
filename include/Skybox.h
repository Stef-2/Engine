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

        inline void SetShader(const Engine::Shader& shader);
        inline void SetTexture(const Engine::Texture& texture);

        inline Engine::Shader& GetShader();
        inline Engine::Texture& GetTexture();

        // handles for Vertex Buffer
        // needs to be passed to OpenGL functions that are supposed to draw the mesh
        inline unsigned int GetVBO();
        inline unsigned int GetVAO();

        // cubemap vertices
        inline float* GetVertices();

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
