#ifndef SKYBOX_H
#define SKYBOX_H

#include "Object.h"
#include "Texture2D.h"
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

        void SetShader(const Engine::ShaderProgram& shader);
        void SetTexture(Engine::Texture2D& texture);

        // given current implementation, it makes no sense to ever have more than one skybox
        // so we're providing an easy way to retrieve and set it from wherever
        static Skybox& GetActiveSkybox();
        static void SetActiveSkybox(Skybox& skybox);

        Engine::ShaderProgram& GetShader();
        Engine::Texture2D& GetTexture();

        // handles for Vertex Buffer
        // needs to be passed to OpenGL functions that are supposed to draw the mesh
        unsigned int GetVBO();
        unsigned int GetVAO();

        // cubemap vertices
        float* GetVertices();

        // setup and prepare the skybox for rendering
        void Setup();
        // set this skybox as the current one
        void SetActive();

    private:
        std::shared_ptr<Engine::Texture2D> texture;
        std::shared_ptr < Engine::ShaderProgram> shader;

        // vertex  and element buffer objects
        unsigned int VAO;
        unsigned int VBO;

        std::unique_ptr<float*> vertices;

        static Skybox* activeSkybox;
    };

}
#endif //  SKYBOX_H
