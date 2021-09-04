#include "Model.h"

Engine::Model::Model()
{
	this->meshes = {};
	this->materials = {};
}

Engine::Model::Model(std::string filePath)
{
	this->meshes = {};
	this->materials = {};

	this->LoadMesh(filePath);
}

void Engine::Model::LoadMesh(std::string filePath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile
                        (filePath,
                        aiProcess_DropNormals | aiProcess_GenSmoothNormals | 
                        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                        aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | 
                        aiProcess_GenBoundingBoxes | aiProcess_ImproveCacheLocality);

    // make sure the import succeeded
    if (!scene) {
        std::cerr << importer.GetErrorString() << std::endl;
        return;
    }

    // utility lambda function that transforms an assimp aiMatrix4x4 into glm::mat4
    auto AiMatrixToGlm = [](aiMatrix4x4 matrix) {

        return glm::mat4 { matrix.a1, matrix.a2, matrix.a3, matrix.a4,
                           matrix.b1, matrix.b2, matrix.b3, matrix.b4,
                           matrix.c1, matrix.c2, matrix.c3, matrix.c4,
                           matrix.d1, matrix.d2, matrix.d3, matrix.d4 };
    };

    // utility vectors to be filled in with data
    std::vector<Engine::VertexBoneData> vertexBoneData {};
    std::vector<Engine::Bone> bones;
    Engine::Skeleton skeleton{};

    std::vector<Engine::Vertex> vertices {};
    std::vector<unsigned int> indices {};

    // utility vectors for manual bounding box building
    glm::vec3 min{ 0.0f };
    glm::vec3 max{ 0.0f };

    // debug vars for vertex / polycount sanity checks
    unsigned int vertexCount = 0;
    unsigned int triangleCount = 0;

    // assimp native empty vector3, to be used for any missing vertex data so its not left uninitialized
    aiVector3D empty{ 0.0f, 0.0f, 0.0f };

    // parse animation data
    if (scene->HasAnimations())
    {
        aiAnimation* animation;
        std::cout << "Model: " << filePath << " has animation data." << std::endl;
        for (size_t i = 0; i < scene->mNumAnimations; i++)
        {
            animation = scene->mAnimations[i];
            // animation->mChannels[i].
        }
    }
    
    // go through all the meshes
    for (size_t i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        vertexCount += mesh->mNumVertices;
        triangleCount += mesh->mNumFaces;
        vertices.clear();
        indices.clear();

        // accumulate min and max values of bounding boxes from all parsed meshes since we want one bounding box for the entire model
        min = glm::min(min, glm::vec3(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z));
        max = glm::max(max, glm::vec3(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z));

        // fill the vertex bone data struct with full range of blank data
        if (mesh->HasBones()) vertexBoneData.assign(mesh->mNumVertices, Engine::VertexBoneData{});

        // go through all mesh vertices
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            // make sure the required data is present and construct a vertex out of it
            // in case data isn't there for whatever reason, substitute an empty vec3 so we don't end up with uninitialized values
            aiVector3D position = mesh->HasPositions() ? mesh->mVertices[j] : empty;
            aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[j] : empty;
            aiVector3D uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][j] : empty;

                                   // positions
            Engine::Vertex vertex{ {position.x, position.y, position.z},
                                   // normals
                                   {normal.x, normal.y, normal.z},
                                   // UVs
                                   {uv.x, uv.y} };

            // push it onto the stack
            vertices.push_back(vertex);

            // in case the mesh has bones, parse them for weight data
            if (mesh->HasBones())
            {
                // go through all the bones
                for (size_t k = 0; k < mesh->mNumBones; k++)
                {
                    aiBone* bone = mesh->mBones[k];

                    // go through all the weights for a given bone
                    for (size_t l = 0; l < bone->mNumWeights; l++)
                    {
                        // check if the weight ID and vertex index match
                        if (bone->mWeights[l].mVertexId == j)
                        {
                            // push new vertex bone data for a matching vertex 
                            vertexBoneData.at(j).boneID.push_back(k);
                            vertexBoneData.at(j).boneWeight.push_back(bone->mWeights->mWeight);
                        }
                    }
                }
            }
        }

        // go through all mesh faces / triangles and form indices
        for (size_t j = 0; j < mesh->mNumFaces; j++) {

            indices.push_back(mesh->mFaces[j].mIndices[0]);
            indices.push_back(mesh->mFaces[j].mIndices[1]);
            indices.push_back(mesh->mFaces[j].mIndices[2]);
        }

        // parse bones if they're present
        if (mesh->HasBones())
        {
            aiNode* rootNode = scene->mRootNode;

            // pass the inverted root node transform to mesh skeleton as the global inverse matrix, needed for correct placement of bones
            skeleton.SetGlobalInverseMatrix(glm::inverse(AiMatrixToGlm(rootNode->mTransformation)));
            
            // go through all the bones
            for (size_t j = 0; j < mesh->mNumBones; j++)
            {
                aiBone* bone = mesh->mBones[j];
                
                // convert the offset matrix into a glm one
                glm::mat4 matrix = AiMatrixToGlm(mesh->mBones[j]->mOffsetMatrix);

                // find the scene node with a matching name
                aiNode* match = rootNode->FindNode(bone->mName);
                
                // push a new bone to the stack
                bones.push_back(Engine::Bone(bone->mName.C_Str(), AiMatrixToGlm(bone->mOffsetMatrix), bone->mNumWeights));

                // start at the found match and work our way up the tree until we find the root
                while (match->mName != rootNode->mName)
                {
                    match = match->mParent;

                    // add up all parent transformations until we reach the end
                    bones.back().AddInheritedTransform(AiMatrixToGlm(match->mTransformation));
                }
            }
        }

        // fully made mesh with no animation
        this->meshes.push_back(Mesh(vertices, indices));

    }

    // debug
    std::cout << "model: " << filePath << " vertex count: " << vertexCount << std::endl;
    std::cout << "model: " << filePath << " triangle count: " << triangleCount << std::endl;

    // set the final bounding box for the entire model
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