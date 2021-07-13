#include "Mesh.h"

Engine::Mesh::Mesh()
{
    this->vertices = {};
    this->indices = {};
    this->VBO = 0;
    this->EBO = 0;
}

Engine::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    this->Setup();
}

void Engine::Mesh::Setup()
{
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //vertex pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    //vertex UVs
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    //element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Engine::Mesh::Draw(Shader* shader)
{
    unsigned int pos0 = glGetAttribLocation(shader->GetProgramID(), "vertPos");
    glBindAttribLocation(shader->GetProgramID(), pos0, "vertPos");

    unsigned int pos1 = glGetAttribLocation(shader->GetProgramID(), "vertCoord");
    glBindAttribLocation(shader->GetProgramID(), pos1, "vertCoord");

    //vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //vertex pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
 
    //vertex UVs
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(1);
    
    //element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    shader->Run();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

Engine::Mesh::~Mesh()
{
    //if(this->vertexBuffer != NULL) delete this->vertexBuffer;
    //if(this->indices != NULL) delete this->indices;
}

Engine::Mesh::Mesh(const Mesh& other)
{
    this->VBO = other.VBO;
    this->EBO = other.EBO;
    this->vertices = other.vertices;
    this->indices = other.indices;
}

Engine::Mesh& Engine::Mesh::operator=(const Mesh& rhs)
{
    if (this == &rhs) return *this;
    return *this;
}
