#ifndef MESH_H
#define MESH_H

#define STB_IMAGE_IMPLEMENTATION

#include "Shader.h"
#include "Material.h"

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
        glm::uvec3 boneID;
        glm::vec3 boneWeight;
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
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<VertexBoneData> bones);

            void Setup();

            // handles for Vertex Buffer and Element Buffer objects
            // they need to be passed to OpenGL functions that are supposed to draw the mesh
            unsigned int GetVBO();
            unsigned int GetEBO();

            std::vector<Vertex>* GetVertices();
            std::vector<unsigned int>* GetIndices();
            std::vector<Triangle> GetTriangles();
            std::vector<VertexBoneData> GetBones();

            void SetVertices(std::vector<Vertex> vertices);
            void SetIndices(std::vector<unsigned int> indices);


        private:
            // mesh vertices
            std::vector<Vertex> vertices;
            std::vector<VertexBoneData> bones;

            // order in which they connect to form triangles
            std::vector<unsigned int> indices;

            std::vector<Triangle> triangles;

            // vertex buffer and element buffer objects
            unsigned int VBO;
            unsigned int EBO;
    };

}

#endif //  MESH_H
