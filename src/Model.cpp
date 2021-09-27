#include "Model.h"

Engine::Model::Model()
{
	this->staticMeshes = {};
    this->animatedMeshes = {};
}

Engine::Model::Model(std::string filePath)
{
	this->staticMeshes = {};
    this->animatedMeshes = {};

	this->LoadMesh(filePath);
}

void Engine::Model::LoadMesh(std::string filePath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile
    (filePath,
                aiProcess_GenSmoothNormals  |
                aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes |
                aiProcess_GenBoundingBoxes | aiProcess_ImproveCacheLocality);

    importer.ApplyPostProcessing(aiProcess_CalcTangentSpace);

    // make sure the import succeeded
    if (!scene) {
        std::cerr << importer.GetErrorString() << std::endl;
        return;
    }

    // utility lambda that transforms assimp's native aiMatrix4x4 into a glm::mat4 one
    auto AiMatrix4ToGlm = [](aiMatrix4x4 matrix) {

        return glm::mat4{ matrix.a1, matrix.b1, matrix.c1, matrix.d1,
                          matrix.a2, matrix.b2, matrix.c2, matrix.d2,
                          matrix.a3, matrix.b3, matrix.c3, matrix.d3,
                          matrix.a4, matrix.b4, matrix.c4, matrix.d4 };
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
                        return Engine::Bone(AiMatrix4ToGlm(bone->mOffsetMatrix), bone->mNumWeights, j);
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
        return glm::quat(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
    };

    // recursive utility lambda that constructs an Engine::Node tree out of assimp's native aiNode tree and returns the root
    std::function<Engine::Node* (aiNode*, Engine::Node*, std::map<aiNode*, bool>, std::vector<Engine::Bone>&, std::vector<Engine::Animation>&)>
        aiTreeToNode = [&aiTreeToNode, &AiMatrix4ToGlm, &FindNodeAnimation, &aiVector3ToGlm, &aiQuaternionToGlm, &FindNodeBone, &scene]
        (aiNode* aiRoot, Engine::Node* rootNode, std::map<aiNode*, bool> map, std::vector<Engine::Bone>& bones, std::vector<Engine::Animation>& animations)
    {
        // check if we've reached the leaves
        if (!aiRoot->mNumChildren) {

            auto root = rootNode;

            //traverse the tree up to the root and return it
            while (root->GetParent())
                root = root->GetParent();

            return root;
        }

        // if we haven't, keep traversing down the tree
        for (size_t i = 0; i < aiRoot->mNumChildren; i++)
        {
            // check the map to see if the node is neccessary for the skeleton
            if (map.at(aiRoot->mChildren[i]))
            {
                // try to find an appropriate bone for the node, if it doesn't exist, make a new one
                Engine::Bone bone = FindNodeBone(scene, aiRoot->mChildren[i]);

                // find the animation corresponding to this node
                aiNodeAnim* nodeAnimation = FindNodeAnimation(scene, aiRoot->mChildren[i]);

                // create a child and give it the bone we made
                rootNode->AddChild(new Engine::Node(aiRoot->mChildren[i]->mName.C_Str(), AiMatrix4ToGlm(aiRoot->mChildren[i]->mTransformation)));

                // associate the node with an animation node in the animation vector
                if (nodeAnimation)
                {
                    // go through all the animations
                    for (unsigned j = 0; j < scene->mNumAnimations; j++)
                    {
                        aiAnimation& aiAnim = *scene->mAnimations[j];

                        // parse animNodes (channels)
                        for (size_t k = 0; k < aiAnim.mNumChannels; k++)
                        {
                            aiNodeAnim* aiNodeAnim = aiAnim.mChannels[k];

                            if (aiNodeAnim == FindNodeAnimation(scene, aiRoot->mChildren[i]))
                                animations.at(j).GetNodeAnimations().at(k).node = rootNode->GetChildren().back();
                        }
                    }
                }

                // if the found bone is valid, add it onto the pile
                if (bone.GetNumAffectedVertices()) {
                    bone.SetNode(*rootNode->GetChildren().back());
                    bones.push_back(bone);
                }

                // set ourselves as the parent
                rootNode->GetChildren().back()->SetParent(rootNode);

                // pass the call to the youngster
                aiTreeToNode(aiRoot->mChildren[i], rootNode->GetChildren().back(), map, bones, animations);
            }
        }
    };

    // utility vectors to be filled with data
    std::vector<Engine::VertexBoneData> vertexBoneData{};

    std::vector<Engine::Animation> animations;
    std::vector<Engine::Vertex> vertices{};
    std::vector<unsigned int> indices{};
    std::vector<Engine::Bone> bones;
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
    aiNode* meshNode = nullptr;

    // parse animation data and fill in the animations vector
    if (scene->HasAnimations())
    {
        // go through all the animations
        for (unsigned i = 0; i < scene->mNumAnimations; i++)
        {
            aiAnimation& aiAnim = *scene->mAnimations[i];

            // push back a new animation
            animations.push_back(Engine::Animation(aiAnim.mName.C_Str(), aiAnim.mDuration, aiAnim.mTicksPerSecond));

            // parse animNodes (channels)
            for (size_t j = 0; j < aiAnim.mNumChannels; j++)
            {
                aiNodeAnim* aiNodeAnim = aiAnim.mChannels[j];

                // push back a new node animation
                animations.back().AddNodeAnimation(Engine::NodeAnimation());
                
                for (size_t i = 0; i < aiNodeAnim->mNumPositionKeys; i++)
                    animations.back().GetNodeAnimations().back().positionKeyFrames.push_back
                    (Engine::VectorKeyFrame{ aiVector3ToGlm(aiNodeAnim->mPositionKeys[i].mValue), aiNodeAnim->mPositionKeys[i].mTime });

                for (size_t i = 0; i < aiNodeAnim->mNumRotationKeys; i++)
                    animations.back().GetNodeAnimations().back().rotationKeyFrames.push_back
                    (Engine::QuaternionKeyFrame{ aiQuaternionToGlm(aiNodeAnim->mRotationKeys[i].mValue), aiNodeAnim->mRotationKeys[i].mTime });

                for (size_t i = 0; i < aiNodeAnim->mNumScalingKeys; i++)
                    animations.back().GetNodeAnimations().back().scaleKeyFrames.push_back
                    (Engine::VectorKeyFrame{ aiVector3ToGlm(aiNodeAnim->mScalingKeys[i].mValue), aiNodeAnim->mScalingKeys[i].mTime });
            }
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
        if (mesh->HasBones()) vertexBoneData.assign
            (mesh->mNumVertices, Engine::VertexBoneData{ glm::vec3(0.0f),glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::ivec4(-1.0), glm::vec4(0.0f) });

        // go through all mesh vertices
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            // make sure the required data is present and construct a vertex out of it
            // in case data isn't there for whatever reason, substitute an empty vec3 so we don't end up with uninitialized values
            aiVector3D position = mesh->HasPositions() ? mesh->mVertices[j] : empty;
            aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[j] : empty;
            aiVector3D bitangent = mesh->HasTangentsAndBitangents() ? mesh->mBitangents[j] : empty;
            aiVector3D tangent = mesh->HasTangentsAndBitangents() ? mesh->mTangents[j] : empty;
            aiVector3D uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][j] : empty;
            
            // positions
            Engine::Vertex vertex{ aiVector3ToGlm(position),
            // normals
            aiVector3ToGlm(normal),
            // bitangents
            aiVector3ToGlm(bitangent),
            // tangents
            aiVector3ToGlm(tangent),
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
                            for (short m = 0; m < 4; m++)
                            {
                                // find an empty slot for the new weight, we support up to 4
                                if (vertexBoneData.at(j).boneID[m] == -1) {
                                    vertexBoneData.at(j).boneID[m] = k;
                                    vertexBoneData.at(j).boneWeight[m] = bone->mWeights[l].mWeight;
                                    break;
                                }
                            }

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

        // create node / bone hierarchy for the skeleton, in case the mesh has one
        if (mesh->mNumBones)
        {
            // create a map that will help record which scene nodes are needed for the bone hierarchy
            std::map<aiNode*, bool> nodeMap;
            aiNode* meshNode = nullptr;

            // recursive lambda that will fill the map with the node tree and set all values to false
            std::function<void(aiNode*)> FillNodeMap = [&nodeMap, &FillNodeMap](aiNode* root)
            {
                // check if we've reached the leaves
                if (!root->mNumChildren)
                    return;

                // else, recursively iterate through the tree and initialize every map value with a false
                for (size_t i = 0; i < root->mNumChildren; i++)
                {
                    nodeMap.insert(std::pair<aiNode*, bool>(root->mChildren[i], false));
                    FillNodeMap(root->mChildren[i]);
                }
            };

            // set the first map entry manually since we're using it as root for the mapping lambda
            nodeMap.insert(std::pair<aiNode*, bool>(scene->mRootNode, false));
            // fill the rest of the map
            FillNodeMap(scene->mRootNode);

            // go through the map and set all nodes between bones and mesh root to true, this will mark the skeleton
            for (size_t o = 0; o < mesh->mNumBones; o++)
            {
                // find the node for the current bone
                aiNode* boneNode = scene->mRootNode->FindNode(mesh->mBones[o]->mName);
                // set it to true
                nodeMap.at(boneNode) = true;
                meshNode = nullptr;

                // go through all the node's parents until we find either the mesh node, or its parent node, that is the end of our skeleton
                while (!meshNode)
                {
                    for (size_t j = 0; j < boneNode->mNumChildren; j++)
                        for (size_t k = 0; k < boneNode->mChildren[j]->mNumMeshes; k++)
                            if (boneNode->mChildren[j]->mMeshes[k] == i) {
                                meshNode = boneNode;
                                break;
                            }


                    // set the full range of nodes to true, they are needed for the skeleton
                    nodeMap.at(boneNode) = true;

                    // check if we've reached the top
                    if (boneNode == scene->mRootNode) {
                        meshNode = scene->mRootNode;
                        break;
                    }

                    // otherwise, keep going up
                    else
                        boneNode = boneNode->mParent;
                }
            }

            // assemble Mr.Skeltal from the scene's aiNode tree
            auto root = new Engine::Node();
            assert(meshNode);
            aiTreeToNode(meshNode, root, nodeMap, bones, animations);

            // decapacitate the assembled tree since the root node was just a manually made seed with zero data
            root->GetChildren().back()->DeleteAbove();


            skeleton.SetRootNode(root);

            // sort the bone vector for faster retrieval later, their current order is random and irrelevant
            // sort them according to their internal index
            std::sort(bones.begin(), bones.end(), [](const Engine::Bone& x, const Engine::Bone& y) -> bool {return x.GetID() < y.GetID(); });
            skeleton.SetBones(bones);

            // assign the global inverse matrix of the scene, it will be needed for final bone transformations
            glm::mat4 globalInvserse = glm::inverse(AiMatrix4ToGlm(scene->mRootNode->mTransformation));

            skeleton.SetGlobalInverseMatrix(globalInvserse);
        }

        // assemble the whole mesh depending if its animated or not
        if (mesh->HasBones()) {
            
            // combine basic vertex data with bone weight data for animated vertices
            for (size_t i = 0; i < vertices.size(); i++)
            {
                vertexBoneData.at(i).position = vertices.at(i).position;
                vertexBoneData.at(i).normal = vertices.at(i).normal;
                vertexBoneData.at(i).bitangent = vertices.at(i).bitangent;
                vertexBoneData.at(i).tangent = vertices.at(i).tangent;
                vertexBoneData.at(i).uv = vertices.at(i).uv;
            }

            // animated mesh
            this->animatedMeshes.push_back(AnimatedMesh(vertexBoneData, indices, skeleton, animations));
        }
        else
            // static mesh with no animations nor bones
            this->staticMeshes.push_back(Mesh(vertices, indices));
    }

    // debug
    std::cout << "model: " << filePath << " vertex count: " << vertexCount << std::endl;
    std::cout << "model: " << filePath << " triangle count: " << triangleCount << std::endl;
    std::cout << "model: " << filePath << " bone count: " << boneCount << std::endl;
    std::cout << "model: " << filePath << " animation count: " << scene->mNumAnimations << std::endl;

    // set the final bounding box for the entire model
    this->SetBoundingBox(min, max);
}

void Engine::Model::LoadMesh(const Engine::Mesh& other)
{
    this->staticMeshes.push_back(other);
}

void Engine::Model::LoadMesh(const Engine::AnimatedMesh& other)
{
    this->animatedMeshes.push_back(other);
}

void Engine::Model::SetBoundingBox(glm::vec3 mins, glm::vec3 maxs)
{
    this->boundingBox = Engine::BoundingBox{mins, maxs};
}


std::vector<Engine::Mesh>& Engine::Model::GetStaticMeshes()
{
    return this->staticMeshes;
}

std::vector<Engine::AnimatedMesh>& Engine::Model::GetAnimatedMeshes()
{
    return this->animatedMeshes;
}

Engine::BoundingBox& Engine::Model::GetBoundingBox()
{
    return this->boundingBox;
}