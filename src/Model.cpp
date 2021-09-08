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

    // utility lambda that transforms assimp's native aiMatrix4x4 into a glm::mat4 one
    auto AiMatrix4ToGlm = [](aiMatrix4x4 matrix) {

        return glm::mat4{ matrix.a1, matrix.a2, matrix.a3, matrix.a4,
                          matrix.b1, matrix.b2, matrix.b3, matrix.b4,
                          matrix.c1, matrix.c2, matrix.c3, matrix.c4,
                          matrix.d1, matrix.d2, matrix.d3, matrix.d4 };
    };

    // utility lambda that finds assimp's native aiNodeAnim for a given aiNode name
    auto FindNodeAnimation = [](const aiScene* scene, aiNode* node) -> aiNodeAnim*
    {
        for (size_t i = 0; i < scene->mNumAnimations; i++)
        {
            aiAnimation& animation = *scene->mAnimations[i];

            for (size_t j = 0; j < animation.mNumChannels; j++)
            {
                aiNodeAnim* nodeAnim = animation.mChannels[j];

                // nodes and their respective animations are connected through names
                if (nodeAnim->mNodeName == node->mName)
                    return nodeAnim;
            }
        }

        // not all nodes have animations, if we couldn't find one, make an empty one
        return nullptr;
    };

    // utility lambda that finds assimp's native aiBone for a given aiNode name
    auto FindNodeBone = [&AiMatrix4ToGlm](const aiScene* scene, aiNode* node) -> Engine::Bone
    {
        for (size_t i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh& mesh = *scene->mMeshes[i];

            // make sure the mash actually has bone data
            if (mesh.HasBones())
                for (size_t j = 0; j < mesh.mNumBones; j++)
                {
                    aiBone* bone = mesh.mBones[j];

                    // compare names
                    if (bone->mName == node->mName)
                        return Engine::Bone(bone->mName.C_Str(), AiMatrix4ToGlm(bone->mOffsetMatrix), bone->mNumWeights, j);
                }
        }

        // not every node has a bone
        // in case we couldn't find a matching one, throw back a fresh one
        return Engine::Bone();
    };

    // utility lambda that converts assimp's native aiVector3D into a glm::vec3 one
    auto aiVector3ToGlm = [](aiVector3D vector)
    {
        return glm::vec3(vector.x, vector.y, vector.z);
    };

    // utility lambda that converts assimp's native aiQuaternion into a glm::quat one
    auto aiQuaternionToGlm = [](aiQuaternion quaternion)
    {
        return glm::quat(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
    };

    // recursive utility lambda that constructs an Engine::Node<Engine::Bone> tree out of assimp's native aiNode tree and returns the root
    std::function<Engine::Node<Engine::Bone>*(aiNode*, Engine::Node<Engine::Bone>*)>
    aiTreeToNode = [&aiTreeToNode, &AiMatrix4ToGlm, &FindNodeAnimation, &aiVector3ToGlm, &aiQuaternionToGlm, &FindNodeBone, &scene]
                   (aiNode* aiNode, Engine::Node<Engine::Bone>* rootNode)
    {
        // check if we've reached the leaves
        if (!aiNode->mNumChildren) {

            auto root = rootNode;

            //traverse the tree up to the root and return it
            while (root->GetParent())
                root = root->GetParent();

            return root;
        }
        
        // if we haven't, keep traversing down the tree
        for (size_t i = 0; i < aiNode->mNumChildren; i++)
        {
            // try to find an appropriate bone for the node, if it doesn't exist, make a new one
            Engine::Bone bone = FindNodeBone(scene, aiNode->mChildren[i]);

            // find the animation corresponding to this node
            aiNodeAnim* nodeAnimation = FindNodeAnimation(scene, aiNode->mChildren[i]);

            // create a child and give it the bone we made
            rootNode->AddChild(new Engine::Node<Engine::Bone>(aiNode->mChildren[i]->mName.C_Str(), AiMatrix4ToGlm(aiNode->mChildren[i]->mTransformation)));

            // if we managed to find the animation for the node, write the animation keyframes into it
            // extract position, rotation, and scale animation keys
            if (nodeAnimation)
            {
                for (size_t i = 0; i < nodeAnimation->mNumPositionKeys; i++)
                    rootNode->GetChildren().back()->GetPositionKeyFrames().push_back(Engine::VectorKeyFrame{ aiVector3ToGlm(nodeAnimation->mPositionKeys[i].mValue), nodeAnimation->mPositionKeys[i].mTime });

                for (size_t i = 0; i < nodeAnimation->mNumRotationKeys; i++)
                    rootNode->GetChildren().back()->GetRotationKeyFrames().push_back(Engine::QuaternionKeyFrame{ aiQuaternionToGlm(nodeAnimation->mRotationKeys[i].mValue), nodeAnimation->mRotationKeys[i].mTime });

                for (size_t i = 0; i < nodeAnimation->mNumScalingKeys; i++)
                    rootNode->GetChildren().back()->GetScaleKeyFrames().push_back(Engine::VectorKeyFrame{ aiVector3ToGlm(nodeAnimation->mScalingKeys[i].mValue), nodeAnimation->mScalingKeys[i].mTime });
            }

            // if the found bone is valid, add it
            if (bone.GetNumAffectedVertices())
                rootNode->GetChildren().back()->AddData(bone);

            // set ourselves as the parent
            rootNode->GetChildren().back()->SetParent(rootNode);
            // pass the call to the youngster
            aiTreeToNode(aiNode->mChildren[i], rootNode->GetChildren().back());
        }
    };

    // utility vectors to be filled with data
    std::vector<Engine::VertexBoneData> vertexBoneData{};

    std::vector<Engine::Vertex> vertices{};
    std::vector<unsigned int> indices{};

    Engine::Skeleton skeleton{};

    // utility vectors for manual bounding box building
    glm::vec3 min{ 0.0f };
    glm::vec3 max{ 0.0f };

    // debug vars for vertex / polycount sanity checks
    unsigned int vertexCount = 0;
    unsigned int triangleCount = 0;
    unsigned int boneCount = 0;

    // assimp native empty vector3, to be used for any missing vertex data so its not left uninitialized
    aiVector3D empty{ 0.0f, 0.0f, 0.0f };

    // parse animation data
    if (scene->HasAnimations())
    {
        // go through all the animations
        for (unsigned i = 0; i < scene->mNumAnimations; i++)
        {
            aiAnimation* aiAnim = scene->mAnimations[i];

            // assemble and push the animation
            this->AddAnimation(Engine::Animation(aiAnim->mName.C_Str(), aiAnim->mDuration, aiAnim->mTicksPerSecond));
        }
    }
    
    // go through all the meshes
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];

        vertexCount += mesh->mNumVertices;
        triangleCount += mesh->mNumFaces;
        boneCount += mesh->mNumBones;

        vertices.clear();
        indices.clear();
        skeleton = {};

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
            Engine::Vertex vertex{ aiVector3ToGlm(position),
                                          // normals
                                    aiVector3ToGlm(normal),
                                           // UVs
                                    glm::vec2(aiVector3ToGlm(uv)) };

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
        
        // assemble Mr.Skeltal from the scene's aiNode tree
        auto root = new Engine::Node<Engine::Bone>();
        aiTreeToNode(scene->mRootNode, root);

        skeleton.SetRootNode(root);
        
        // assemble the whole model depending if its animated or not
        if (mesh->HasBones()) {
            // animated mesh
            this->GetMeshes().push_back(Mesh(vertices, indices, vertexBoneData, skeleton));
        }
        else
            // static made mesh with no animation nor bones
            this->meshes.push_back(Mesh(vertices, indices));
    }

    // debug
    std::cout << "model: " << filePath << " vertex count: " << vertexCount << std::endl;
    std::cout << "model: " << filePath << " triangle count: " << triangleCount << std::endl;
    std::cout << "model: " << filePath << " bone count: " << boneCount << std::endl;
    std::cout << "model: " << filePath << " animation count: " << scene->mNumAnimations << std::endl;

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



void Engine::Model::AddAnimation(Engine::Animation animation)
{
    this->animations.push_back(animation);
}

std::vector<Engine::Mesh>& Engine::Model::GetMeshes()
{
    return this->meshes;
}

std::vector<Engine::Material>& Engine::Model::GetMaterials()
{
    return this->materials;
}

Engine::BoundingBox& Engine::Model::GetBoundingBox()
{
    return this->boundingBox;
}

std::vector<Engine::Animation>& Engine::Model::GetAnimations()
{
    return this->animations;
}