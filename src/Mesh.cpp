#include "Mesh.h"

Engine::Mesh::Mesh()
{
    this->vertices = {};
    this->indices = {};
    this->triangles = {};
    //this->boundingBox = {};
    this->VBO = 0;
    this->EBO = 0;
}

Engine::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;
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

/*Engine::BoundingBox Engine::Mesh::GetBoundingBox()
{
    return this->boundingBox;
}*/

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

    //std::vector<Engine::Triangle> tris;

    for (unsigned int i = 0; i < this->indices.size() - 2; i += 3)
        this->triangles.push_back({ &this->vertices.at(indices.at(i)), &this->vertices.at(indices.at(i + 1)), &this->vertices.at(indices.at(i + 2)) });

    //this->triangles = tris;
}

Engine::Mesh::~Mesh()
{
    //if(this->vertexBuffer != NULL) delete this->vertexBuffer;
    //if(this->indices != NULL) delete this->indices;
    //if (this->VBO) glDeleteBuffers(1, &this->VBO);
    //if (this->EBO) glDeleteBuffers(1, &this->EBO);
}   

/*Engine::Mesh::Mesh(const Mesh& other)
{
    this->VBO = other.VBO;
    this->EBO = other.EBO;
    this->vertices = other.vertices;
    this->indices = other.indices;
}*/

/*Engine::Mesh& Engine::Mesh::operator=(const Mesh& rhs)
{
    if (this == &rhs) return *this;
    return *this;
}*/
