#include "Model.h"

Engine::Model::Model()
{

	this->meshes = {};
	this->materials = {};
}

Engine::Model::Model(const char* filePath)
{
	this->meshes = {};
	this->materials = {};

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
    int nLoadedMats = loader.LoadedMaterials.size();
    std::cout << "materials: " << nLoadedMats << std::endl;
    std::vector<Vertex> vertices;
    Texture tex;
    
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

void Engine::Model::LoadMaterial(Engine::Material material)
{
    materials.push_back(Material());
}

std::vector<Engine::Mesh>* Engine::Model::GetMeshes()
{
    return &this->meshes;
}

std::vector<Engine::Material>* Engine::Model::GetMaterials()
{
    return &this->materials;
}

void Engine::Model::Draw(Engine::Shader* shader)
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader, &materials[0]);
    }
}