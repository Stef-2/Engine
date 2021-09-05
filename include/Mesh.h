#ifndef MESH_H
#define MESH_H

#define STB_IMAGE_IMPLEMENTATION

#include "Shader.h"
#include "Material.h"
#include "Skeleton.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glfw3.h"

#include "iostream"
#include "vector"

namespace Engine
{
    // basic vertex data
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };
 
    // vertex extension for animated meshes
    struct VertexBoneData
    {
        std::vector<unsigned int> boneID;
        std::vector<float> boneWeight;
    };

    // triangle data for collision detection
    struct Triangle
    {
        Engine::Vertex* a;
        Engine::Vertex* b;
        Engine::Vertex* c;
    };

    // a mesh class that stores vertex data to be used for rendering
    class Mesh
    {
        public:
            Mesh();
            
            // non animatied mesh
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

            // animated mesh
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Engine::Skeleton& skeleton, std::vector<VertexBoneData> boneWeights);

            void Setup();

            // handles for Vertex Buffer and Element Buffer objects
            // they need to be passed to OpenGL functions that are supposed to draw the mesh
            unsigned int GetVAO();
            unsigned int GetVBO();
            unsigned int GetEBO();

            std::vector<Vertex>& GetVertices();
            std::vector<unsigned int>& GetIndices();
            std::vector<Triangle>& GetTriangles();
            std::vector<VertexBoneData>& GetBoneWeights();
            Engine::Skeleton& GetSkeleton();

            void SetVertices(std::vector<Vertex> vertices);
            void SetIndices(std::vector<unsigned int> indices);
            void SetBoneWeights(std::vector<VertexBoneData> boneWeights);
            void SetSkeleton(Engine::Skeleton& skelly);


        private:
            // Mr.Skeltal
            Engine::Skeleton skeleton;

            // mesh vertices
            std::vector<Vertex> vertices;

            // vertex data extension that describes connection between vertices and bone(s) affecting them
            std::vector<VertexBoneData> boneWeights;

            // order in which they connect to form triangles
            std::vector<unsigned int> indices;

            // triangles for collision detection
            std::vector<Triangle> triangles;

            // array, vertex buffer and element buffer objects for rendering
            unsigned int VAO;
            unsigned int VBO;
            unsigned int EBO;
    };

}

#endif //  MESH_H
