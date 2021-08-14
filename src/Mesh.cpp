#include "Mesh.h"

Engine::Mesh::Mesh()
{
    this->vertices = {};
    this->indices = {};
    this->bones = {};
    this->triangles = {};
    this->VBO = 0;
    this->EBO = 0;
}

Engine::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;
    this->bones = {};
    this->triangles = {};
    this->VBO = 0;
    this->EBO = 0;

    this->Setup();
}

Engine::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<VertexBoneData> bones)
{
    this->vertices = vertices;
    this->indices = indices;
    this->bones = bones;
    this->triangles = {};
    this->VBO = 0;
    this->EBO = 0;

    this->Setup();
}

unsigned int Engine::Mesh::GetVBO()
{
    return this->VBO;
}

unsigned int Engine::Mesh::GetEBO()
{
    return this->EBO;
}

std::vector<Engine::Vertex>* Engine::Mesh::GetVertices()
{
    return &this->vertices;
}

std::vector<unsigned int>* Engine::Mesh::GetIndices()
{
    return &this->indices;
}

std::vector<Engine::Triangle> Engine::Mesh::GetTriangles()
{
    return this->triangles;
}

std::vector<Engine::VertexBoneData> Engine::Mesh::GetBones()
{
    return this->bones;
}

void Engine::Mesh::SetVertices(std::vector<Engine::Vertex> vertices)
{
    this->vertices = vertices;
}

void Engine::Mesh::SetIndices(std::vector<unsigned int> indices)
{
    this->indices = indices;
}

void Engine::Mesh::Setup()
{
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //fill the vertex buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //bind the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //fill the element buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //construct triangle data out of vertices and indices
    for (size_t i = 0; i < this->indices.size() - 2; i += 3)
        this->triangles.push_back({ &this->vertices.at(indices.at(i)), &this->vertices.at(indices.at(i + 1)), &this->vertices.at(indices.at(i + 2)) });
}
