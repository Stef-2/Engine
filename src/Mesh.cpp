#include "Mesh.h"

Engine::Mesh::Mesh()
{
    this->vertices = {};
    this->indices = {};
    this->triangles = {};
    this->VAO = 0;
    this->VBO = 0;
    this->EBO = 0;
    this->node = {};
}

Engine::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;
    this->triangles = {};
    this->VAO = 0;
    this->VBO = 0;
    this->EBO = 0;

    this->Setup();
}

Engine::AnimatedMesh::AnimatedMesh(std::vector<VertexBoneData> vertices, std::vector<unsigned int> indices, Engine::Skeleton skeleton)
{
    this->vertices = vertices;
    this->indices = indices;
    this->skeleton = skeleton;
    this->triangles = {};
    this->VAO = 0;
    this->VBO = 0;
    this->EBO = 0;

    this->Setup();
}

unsigned int Engine::Mesh::GetVAO()
{
    return this->VAO;
}

unsigned int Engine::Mesh::GetVBO()
{
    return this->VBO;
}

unsigned int Engine::Mesh::GetEBO()
{
    return this->EBO;
}

std::vector<Engine::Vertex>& Engine::Mesh::GetVertices()
{
    return this->vertices;
}

Engine::Skeleton& Engine::AnimatedMesh::GetSkeleton()
{
    return this->skeleton;
}

std::vector<unsigned int>& Engine::Mesh::GetIndices()
{
    return this->indices;
}

std::vector<Engine::Triangle<Engine::Vertex>>& Engine::Mesh::GetTriangles()
{
    return this->triangles;
}

std::vector<Engine::Triangle<Engine::VertexBoneData>>& Engine::AnimatedMesh::GetTriangles()
{
    return this->triangles;
}

void Engine::Mesh::SetVertices(std::vector<Engine::Vertex> vertices)
{
    this->vertices = vertices;
}

void Engine::AnimatedMesh::SetSkeleton(Engine::Skeleton& skelly)
{
    this->skeleton = skelly;
}

void Engine::Mesh::SetIndices(std::vector<unsigned int> indices)
{
    this->indices = indices;
}

void Engine::Mesh::Setup()
{
    glGenVertexArrays(1, &this->VAO);

    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    // bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // fill the vertex buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // bind the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // fill the element buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // setup vertex attributes

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // vertex texture coords (UVs)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    // unbind VAO
    glBindVertexArray(0);

    // construct triangle data out of vertices and indices
    for (size_t i = 0; i < this->indices.size() - 2; i += 3)
        this->triangles.push_back({ &this->vertices.at(indices.at(i)), &this->vertices.at(indices.at(i + 1)), &this->vertices.at(indices.at(i + 2)) });
}

void Engine::AnimatedMesh::Setup()
{
    glGenVertexArrays(1, &this->VAO);

    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    // bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // fill the vertex buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexBoneData), &vertices[0], GL_STATIC_DRAW);

    // bind the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // fill the element buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    // setup vertex attributes

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, normal));

    // vertex texture coords (UVs)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, uv));

    // vertex bone IDs
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, boneID));

    // vertex bone weights
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, boneWeight));

    // unbind VAO
    glBindVertexArray(0);

    // construct triangle data out of vertices and indices
    for (size_t i = 0; i < this->indices.size() - 2; i += 3)
        this->triangles.push_back({ &this->vertices.at(indices.at(i)), &this->vertices.at(indices.at(i + 1)), &this->vertices.at(indices.at(i + 2)) });
}