#include "Mesh.h"

Engine::Mesh::Mesh()
{
    this->vertices = {};
    this->indices = {};
    //this->boundingBox = {};
    this->VBO = 0;
    this->EBO = 0;
}

Engine::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //bind the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //fill the element buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Engine::Mesh::Draw(Shader* shader, Material* material)
{
    //find and bind the appropriate shader attribute positions
    unsigned int pos_0 = glGetAttribLocation(shader->GetProgramID(), "vertPos");
    glBindAttribLocation(shader->GetProgramID(), pos_0, "vertPos");

    unsigned int pos_1 = glGetAttribLocation(shader->GetProgramID(), "vertCoord");
    glBindAttribLocation(shader->GetProgramID(), pos_1, "vertCoord");

    //bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    //setup vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
 
    //setup vertex UVs attribute
    //struct members are sequential in memory so we can use offsetof() to find the appropriate data
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);
    
    //bind the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    //bind the corresponding texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->GetDiffuse()->GetTextureID());

    //run the shader program
    shader->Run();

    //wrooom !
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

/*Engine::Mesh::~Mesh()
{
    //if(this->vertexBuffer != NULL) delete this->vertexBuffer;
    //if(this->indices != NULL) delete this->indices;
}*/

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
