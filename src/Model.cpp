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

/*void Engine::Model::LoadMesh(const char* filePath)
{
    objl::Loader loader;
    bool success = loader.LoadFile(filePath);

    if (!success) {
        std::cerr << filePath << ": Mesh loading failed" << std::endl;
        return;
    }

    std::vector<Vertex> vertices;
    glm::vec3 min{ 0.0f };
    glm::vec3 max{ 0.0f };
    
    objl::Mesh asd = loader.LoadedMeshes[0];
    std::cout << asd.Vertices.size() << std::endl;

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

            min = glm::min(min, vertex.position);
            max = glm::max(max, vertex.position);
        }
        
        meshes.push_back(Mesh(vertices, loader.LoadedMeshes[i].Indices));
    }

    this->SetBoundingBox(min, max);
}*/

void Engine::Model::LoadMesh(const char* filePath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filePath, aiProcess_DropNormals | aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                                                       aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_GenBoundingBoxes | aiProcess_ImproveCacheLocality);

    if (!scene) {
        std::cerr << importer.GetErrorString() << std::endl;
        return;
    }

    std::vector<Vertex> vertices {};
    std::vector<unsigned int> indices {};

    glm::vec3 min{ 0.0f };
    glm::vec3 max{ 0.0f };
    unsigned int vertexCount = 0;
    unsigned int triangleCount = 0;
    aiVector3D empty{ 0.0f, 0.0f, 0.0f };

    //bool hasAABB = false;
    
    //go through all the meshes
    for (size_t i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        vertexCount += mesh->mNumVertices;
        triangleCount += mesh->mNumFaces;
        vertices.clear();

        //accumulate min and max values of bounding boxes from all meshes since we want one bounding box for the whole model
        min = glm::min(min, glm::vec3(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z));
        max = glm::max(max, glm::vec3(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z));

        //go through all their vertices
        for (size_t j = 0; j < mesh->mNumVertices; j++)
        {
            //construct a vertex out of loaded data
            aiVector3D position = mesh->mVertices[j];
            aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[j] : empty;
            aiVector3D uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][j] : empty;

                                       //positions
            Engine::Vertex vertex{ {position.x, position.y, position.z},
                                       //normals
                                   {normal.x, normal.y, normal.z},
                                       //UVs
                                   {uv.x, uv.y} };

            //push it onto the stack
            vertices.push_back(vertex);
        }

        //go through all their faces / triangles
        for (size_t k = 0; k < mesh->mNumFaces; k++) {

            indices.push_back(mesh->mFaces[k].mIndices[0]);
            indices.push_back(mesh->mFaces[k].mIndices[1]);
            indices.push_back(mesh->mFaces[k].mIndices[2]);
        }

        meshes.push_back(Mesh(vertices, indices));

    }

    std::cout << "model: " << filePath << " vertex count: " << vertexCount << std::endl;
    std::cout << "model: " << filePath << " triangle count: " << triangleCount << std::endl;

    this->SetBoundingBox(min, max);
}

void Engine::Model::LoadMaterial(const Engine::Material& material)
{
    materials.push_back(material);
}

void Engine::Model::LoadMesh(const Engine::Mesh& other)
{
    this->meshes.push_back(other);
}

void Engine::Model::SetBoundingBox(glm::vec3 mins, glm::vec3 maxs)
{
    this->boundingBox = Engine::BoundingBox{mins, maxs};
}

std::vector<Engine::Mesh>* Engine::Model::GetMeshes()
{
    return &this->meshes;
}

std::vector<Engine::Material>* Engine::Model::GetMaterials()
{
    return &this->materials;
}

Engine::BoundingBox* Engine::Model::GetBoundingBox()
{
    return &this->boundingBox;
}