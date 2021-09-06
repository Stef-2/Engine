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

    // utility lambda function that transforms assimp's native aiMatrix4x4 into a glm::mat4 one
    auto AiMatrix4ToGlm = [](aiMatrix4x4 matrix) {

        return glm::mat4{ matrix.a1, matrix.a2, matrix.a3, matrix.a4,
                           matrix.b1, matrix.b2, matrix.b3, matrix.b4,
                           matrix.c1, matrix.c2, matrix.c3, matrix.c4,
                           matrix.d1, matrix.d2, matrix.d3, matrix.d4 };
    };

    // utility lambda function that finds assimp's native aiNodeAnim for a given aiNode name
    auto FindNodeAnimation = [](const aiScene* scene, aiNode* node) -> aiNodeAnim*
    {
        for (size_t i = 0; i < scene->mNumAnimations; i++)
        {
            aiAnimation& animation = *scene->mAnimations[i];

            for (size_t j = 0; j < animation.mNumChannels; j++)
            {
                aiNodeAnim* nodeAnim = animation.mChannels[j];

                if (nodeAnim->mNodeName == node->mName)
                    return nodeAnim;
            }
        }

        return nullptr;
    };

    // utility lambda function that converts assimp's native aiVector3D into a glm::vec3 one
    auto aiVector3ToGlm = [](aiVector3D vector)
    {
        return glm::vec3(vector.x, vector.y, vector.z);
    };

    // utility lambda function that converts assimp's native aiQuaternion into a glm::quat one
    auto aiQuaternionToGlm = [](aiQuaternion quaternion)
    {
        return glm::quat(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
    };

    // utility vectors to be filled with data
    std::vector<Engine::VertexBoneData> vertexBoneData{};
    std::vector<Engine::Animation> animations{};
    std::vector<Engine::Bone> bones{};

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
    {/*
        // go through all the animations
        for (unsigned i = 0; i < scene->mNumAnimations; i++)
        {
            aiAnimation* aiAnimation = scene->mAnimations[i];

            // go through all the animation channels
            for (unsigned j = 0; j < aiAnimation->mNumChannels; j++)
            {
                aiNodeAnim& nodeAnimation = *aiAnimation->mChannels[j];

                channel.

                // assign the basic animation parameters
                keyFrame.name = keyFrame.name = channel.mNodeName.C_Str();

                // create position keyframes
                for (unsigned k = 0; k < channel.mNumPositionKeys; k++) {
                    aiVectorKey key = channel.mPositionKeys[k];
                    keyFrame.positionKey = { Engine::VectorKeyFrame{ glm::vec3{key.mValue[0], key.mValue[1], key.mValue[2]} , key.mTime } };
                }

                // create rotation keyframes
                for (unsigned k = 0; k < channel.mNumRotationKeys; k++) {
                    aiQuatKey key = channel.mRotationKeys[k];
                    keyFrame.rotationKey = { Engine::QuaternionKeyFrame{ glm::quat{key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w }, key.mTime } };
                }

                // create scale keyframes
                for (unsigned k = 0; k < channel.mNumScalingKeys; k++) {
                    aiVectorKey key = channel.mScalingKeys[k];
                    keyFrame.scaleKey = { Engine::VectorKeyFrame{ glm::vec3{key.mValue[0], key.mValue[1], key.mValue[2]} , key.mTime } };
                }

            }

            // assemble and push the whole animation
            this->AddAnimation(Engine::Animation(aiAnimation->mName.C_Str(), aiAnimation->mDuration, aiAnimation->mTicksPerSecond, keyFrames));
            std::cout << "model: " << filePath << " keyframe count: " << keyFrames.size() << std::endl;
        }*/
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
        bones.clear();
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

        // parse bones if they're present
        if (mesh->HasBones())
        {
            aiNode* rootNode = scene->mRootNode;
            int counter = 0;
            // pass the inverted root node transform to mesh skeleton as the global inverse matrix, needed for correct placement of bones
            skeleton.SetGlobalInverseMatrix(glm::inverse(AiMatrix4ToGlm(rootNode->mTransformation)));

            // go through all the bones
            for (unsigned int b = 0; b < mesh->mNumBones; b++) 
            {
                bool found = false;
                aiBone* aibone = mesh->mBones[b];

                // find the scene node with a matching name
                aiNode* match = rootNode->FindNode(aibone->mName);

                // create Engine native node with a bone attached
                Engine::Node<Engine::Bone>* child = new Engine::Node<Engine::Bone>(match->mName.C_Str(), AiMatrix4ToGlm(match->mTransformation),
                    Engine::Bone(aibone->mName.C_Str(), AiMatrix4ToGlm(aibone->mOffsetMatrix), aibone->mNumWeights, b));


                Engine::Node<Engine::Bone>* parent = nullptr;
                
                // start at the found match and work our way up the tree until we find the mesh root node
                while (!(match->mName != rootNode->mName) != !found)
                {
                    // check if any of the meshes in any of the nodes match with the one we're currently parsing
                    for (unsigned k = 0; k < match->mNumMeshes; k++)
                        if (match->mMeshes[k] == i) {
                            found = true;
                            break;
                        }
                    counter++;
                    // keep going up the tree
                    match = match->mParent;

                    // create Engine native node with a bone attached
                    parent = new Engine::Node<Engine::Bone>(match->mName.C_Str(), AiMatrix4ToGlm(match->mTransformation));

                    // find the aiNodeAnim data corresponding to the currently parsed aiNode
                    aiNodeAnim* nodeAnim = FindNodeAnimation(scene, match);
                        
                    // check if we actually managed to find an appropriate aiNodeAnim
                    if (nodeAnim)
                    {
                        // attach a bone to the node
                        parent->GetData().push_back(Engine::Bone());

                        // fill up the current node's bone with keyFrames from animation data:
                        // position keyframes
                        for (size_t i = 0; i < nodeAnim->mNumPositionKeys; i++)
                            parent->GetData().back().GetPositions().push_back(Engine::VectorKeyFrame{ aiVector3ToGlm(nodeAnim->mPositionKeys[i].mValue),
                                                                                                        nodeAnim->mPositionKeys[i].mTime });
                        // rotation keyframes
                        for (size_t i = 0; i < nodeAnim->mNumRotationKeys; i++)
                            parent->GetData().back().GetRotations().push_back(Engine::QuaternionKeyFrame{ aiQuaternionToGlm(nodeAnim->mRotationKeys[i].mValue),
                                                                                                            nodeAnim->mRotationKeys[i].mTime });
                        // scale keyframes
                        for (size_t i = 0; i < nodeAnim->mNumScalingKeys; i++)
                            parent->GetData().back().GetScales().push_back(Engine::VectorKeyFrame{ aiVector3ToGlm(nodeAnim->mScalingKeys[i].mValue),
                                                                                                        nodeAnim->mScalingKeys[i].mTime });
                        //std::cout << "Cycle: " << counter << std::endl;
                    }
                        
                    // switcharoo
                    child->SetParent(parent);
                    parent->AddChild(child);
                    child = parent;
                }
                    
                skeleton.SetRootNode(*parent);
                this->skeleton = skeleton;

                //delete child;
            }
        }

        aiNode* node = scene->mRootNode;
        std::map<aiNode*, bool> nodeMap;

        do
        {   
            for (size_t k = 0; k < node->mNumChildren; k++)
            {
                nodeMap.emplace(node->mChildren[k], false);
            }
        } while (node->mNumChildren > 0)

        if (mesh->HasBones()) {
            // fully made mesh with animation
            skeleton.SetBones(bones);
            this->meshes.push_back(Mesh(vertices, indices, vertexBoneData));
            this->SetSkeleton(skeleton);
        }
        else
            // fully made mesh with no animation
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

void Engine::Model::SetSkeleton(Engine::Skeleton& skelly)
{
    this->skeleton = skelly;
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

Engine::Skeleton& Engine::Model::GetSkeleton()
{
    return this->skeleton;
}

std::vector<Engine::Animation>& Engine::Model::GetAnimations()
{
    return this->animations;
}