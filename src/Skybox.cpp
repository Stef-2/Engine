#include "Skybox.h"

Engine::Skybox::Skybox()
{
    this->shader = {};
    this->texture = {};
    this->VBO = {};

    this->vertices = std::make_unique<float*>(new float[108]{
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    });
}

void Engine::Skybox::SetShader(const Engine::Shader& shader)
{
	this->shader = shader;
}

void Engine::Skybox::SetTexture(const Engine::Texture& texture)
{
    this->texture = texture;
}

Engine::Shader* Engine::Skybox::GetShader()
{
	return &this->shader;
}

Engine::Texture* Engine::Skybox::GetTexture()
{
	return &this->texture;
}

unsigned int Engine::Skybox::GetVBO()
{
    return this->VBO;
}

float* Engine::Skybox::GetVertices()
{
    return *this->vertices.get();
}

void Engine::Skybox::Setup()
{
    //check if both shader and texture have been setup
    //if (!&this->shader || !&this->texture)
        //return;

    glGenBuffers(1, &this->VBO);

    //bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //fill the vertex buffer with data
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), this->vertices.get(), GL_STATIC_DRAW);
}
