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
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filePath, aiProcess_DropNormals | aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                                                       aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_GenBoundingBoxes | aiProcess_ImproveCacheLocality);

    if (!scene) {
        std::cerr << importer.GetErrorString() << std::endl;
        return;
    }

    //vectors to be filled in with data
    std::vector<Engine::Bone> bones {};
    std::vector<Engine::VertexBoneData> vertexBoneData {};

    std::vector<Engine::Vertex> vertices {};
    std::vector<unsigned int> indices {};

    glm::vec3 min{ 0.0f };
    glm::vec3 max{ 0.0f };
    unsigned int vertexCount = 0;
    unsigned int triangleCount = 0;
    aiVector3D empty{ 0.0f, 0.0f, 0.0f };

    //parse animation data
    if (scene->HasAnimations())
    {
        aiAnimation* animation;

        for (size_t i = 0; i < scene->mNumAnimations; i++)
        {
            animation = scene->mAnimations[i];
            //animation->mChannels[i].
        }
    }
    
    //go through all the meshes
    for (size_t i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        vertexCount += mesh->mNumVertices;
        triangleCount += mesh->mNumFaces;
        vertices.clear();

        //parse bones if they're present
        if (mesh->HasBones())
        {
            for (size_t i = 0; i < mesh->mNumBones; i++)
            {
                aiMatrix4x4 offset = mesh->mBones[i]->mOffsetMatrix;
                

                glm::mat4 matrix{ offset.a1, offset.a2, offset.a3, offset.a4,
                                  offset.b1, offset.b2, offset.b3, offset.b4,
                                  offset.c1, offset.c2, offset.c3, offset.c4,
                                  offset.d1, offset.d2, offset.d3, offset.d4 };

                aiVertexWeight weights = mesh->mBones[i]->mWeights[3];
                Engine::Bone bone(std::string(mesh->mBones[i]->mName.C_Str()), matrix, mesh->mBones[i]->mNumWeights);
                bones.push_back(bone);
            }
        }

        //accumulate min and max values of bounding boxes from all meshes since we want one bounding box for the whole model
        min = glm::min(min, glm::vec3(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z));
        max = glm::max(max, glm::vec3(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z));

        //go through all their vertices
        for (size_t j = 0; j < mesh->mNumVertices; j++)
        {
            //make sure the required data is present and construct a vertex out of it
            aiVector3D position = mesh->HasPositions() ? mesh->mVertices[j] : empty;
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