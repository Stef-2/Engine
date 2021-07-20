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
    objl::Loader loader;
    bool success = loader.LoadFile(filePath);

    if (!success) {
        std::cerr << filePath << ": Mesh loading failed" << std::endl;
        return;
    }

    std::vector<Vertex> vertices;
    
    for (unsigned int i = 0; i < loader.LoadedMeshes.size(); i++)
    {
        vertices.clear();

        //we're able to read up to 4294967295 vertices at maximum here
        //if we need more, then to fix the issue we should fire the guy doing the modeling
        for (unsigned int j = 0; j < loader.LoadedMeshes[i].Vertices.size(); j++)
        {
            //construct the vertex out of loaded positions and UV coordinates
            Vertex vertex{ glm::vec3(loader.LoadedMeshes[i].Vertices[j].Position.X,
                                     loader.LoadedMeshes[i].Vertices[j].Position.Y,
                                     loader.LoadedMeshes[i].Vertices[j].Position.Z),
                           glm::vec2(loader.LoadedMeshes[i].Vertices[j].TextureCoordinate.X,
                                     loader.LoadedMeshes[i].Vertices[j].TextureCoordinate.Y) };

            //push it onto the stack
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
    for (size_t i = 0; i < meshes.size(); i++) {

        meshes[i].Draw(shader, &materials[0]);
    }
}