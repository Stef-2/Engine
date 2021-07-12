#include "Mesh.h"

Engine::Mesh::Mesh()
{
    this->vertexBufferr = {};
    this->indices = {};
    this->VBO = 0;
    this->EBO = 0;
}

Engine::Mesh::Mesh(const char* filePath)
{
    this->vertexBufferr = {};
    this->indices = {};
    this->VBO = 0;
    this->EBO = 0;

    this->Setup(filePath);
}

void Engine::Mesh::Setup(const char* filePath)
{
    bool success = false;
    int counter = 0;

    objl::Loader loader;
    success = loader.LoadFile(filePath);

    if (!success)
    {
        std::cerr << filePath << ": Mesh loading failed" << std::endl;
        this->vertexBufferr = {};
        this->indices = {};
        this->VBO = 0;
        this->EBO = 0;
        return;
    }

    objl::Mesh mesh = loader.LoadedMeshes[0];
    vertexBufferr = {};
    indices = {};

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    for (int i = 0; i < mesh.Vertices.size(); i++)
    {
        vertexBuffer[counter] = mesh.Vertices[i].Position.X;
        vertexBuffer[counter + 1] = mesh.Vertices[i].Position.Y;
        vertexBuffer[counter + 2] = mesh.Vertices[i].Position.Z;

        //vertNrm[counter3] = mesh.Vertices[i].Normal.X;
        //vertNrm[counter3+1] = mesh.Vertices[i].Normal.Y;
        //vertNrm[counter3+2] = mesh.Vertices[i].Normal.Z;

        vertexBuffer[counter + 3] = mesh.Vertices[i].TextureCoordinate.X;
        vertexBuffer[counter + 4] = mesh.Vertices[i].TextureCoordinate.Y;

        counter += 5;
    }

    for (int i = 0; i < mesh.Indices.size(); i++)
        indices[i] = mesh.Indices[i];

    //vertex buffer
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glBindAttribLocation(shaderProgram, 0, "vertPos");
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    //glBindAttribLocation(shaderProgram, 1, "vertColor");
    //glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(5 * sizeof(float)));
    //glBindAttribLocation(shaderProgram, 1, "vertCoord");
    glEnableVertexAttribArray(1);

    //element buffer
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indices), indices, GL_STATIC_DRAW);

    for (int i = 0; i < mesh.Vertices.size() * 5; i++)
        std::cout << "vertex buffer-" << i << ": " << *(this->vertexBuffer + i) << std::endl;
    for (int i = 0; i < mesh.Indices.size(); i++)
        std::cout << "indices-" << i << ": " << *(this->indices + i) << std::endl;
    std::cout << "vb size: " << sizeof(vertexBuffer) / sizeof(vertexBuffer[0]) << std::endl;
    std::cout << "indices size: " << sizeof(indices) / sizeof(indices[0]) << std::endl;
    std::cout << "vertices: " << mesh.Vertices.size() << std::endl;
}

void Engine::Mesh::Draw(Shader* shader)
{
    //vertex buffer
    glGenBuffers(1, &this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glBindAttribLocation(shaderProgram, 0, "vertPos");
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    //glBindAttribLocation(shaderProgram, 1, "vertColor");
    //glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
    //glBindAttribLocation(shaderProgram, 1, "vertCoord");
    glEnableVertexAttribArray(1);

    //element buffer
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indices), indices, GL_STATIC_DRAW);

    unsigned int pos0 = glGetAttribLocation(shader->GetProgramID(), "vertPos");
    glBindAttribLocation(shader->GetProgramID(), pos0, "vertPos");

    unsigned int pos1 = glGetAttribLocation(shader->GetProgramID(), "vertCoord");
    glBindAttribLocation(shader->GetProgramID(), pos1, "vertCoord");

    shader->Run();
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
}

Engine::Mesh::~Mesh()
{
    if(this->vertexBuffer != NULL) delete this->vertexBuffer;
    if(this->indices != NULL) delete this->indices;
}

Engine::Mesh::Mesh(const Mesh& other)
{

}

Engine::Mesh& Engine::Mesh::operator=(const Mesh& rhs)
{
    if (this == &rhs) return *this;
    return *this;
}
