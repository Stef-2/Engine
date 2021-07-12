#include "Model.h"

Engine::Model::Model()
{
	this->meshes = {};
	this->textures = {};
}

Engine::Model::Model(const char* filePath)
{
	this->meshes = {};
	this->textures = {};

	this->LoadMesh(filePath);
}

void Engine::Model::LoadMesh(const char* filePath)
{
    bool success = false;
    int counter = 0;

    objl::Loader loader;
    success = loader.LoadFile(filePath);

    if (!success)
    {
        std::cerr << filePath << ": Mesh loading failed" << std::endl;
        return;
    }

    int nLoadedMeshes = loader.LoadedMeshes.size();
    std::vector<Vertex> vertices;
    
    for (size_t i = 0; i < nLoadedMeshes; i++)
    {
        vertices.clear();

        for (size_t j = 0; j < loader.LoadedMeshes[i].Vertices.size(); j++)
        {
            Vertex vertex{ glm::vec3(loader.LoadedMeshes[i].Vertices[j].Position.X,
                                     loader.LoadedMeshes[i].Vertices[j].Position.Y,
                                     loader.LoadedMeshes[i].Vertices[j].Position.Z),
                           glm::vec2(loader.LoadedMeshes[i].Vertices[j].TextureCoordinate.X,
                                     loader.LoadedMeshes[i].Vertices[j].TextureCoordinate.Y) };

            vertices.push_back(vertex);
        }
        
        meshes.push_back(Mesh(vertices, loader.LoadedMeshes[i].Indices));
    }

}

void Engine::Model::Draw(Engine::Shader* shader)
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}