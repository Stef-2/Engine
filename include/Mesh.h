#ifndef MESH_H
#define MESH_H

#define STB_IMAGE_IMPLEMENTATION
#define OBJL_INCLUDED

#include "Shader.h"
#include "Material.h"

#ifndef OBJL_INCLUDED
#include "OBJ_Loader.h"
#define OBJL_INCLUDED
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glfw3.h"

#include "iostream"
#include "vector"

namespace Engine
{

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct Triangle
    {
        Engine::Vertex a;
        Engine::Vertex b;
        Engine::Vertex c;
    };

    //a mesh class that stores vertex data to be used for rendering
    class Mesh
    {
        public:
            Mesh();
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
            ~Mesh();
            //Mesh(const Mesh& other);
            //Mesh& operator=(const Mesh& other);

            void Setup();
            void Draw(Shader* shader, Material* material);

            //handles for Vertex Buffer and Element Buffer objects
            //they need to be passed to OpenGL functions that are supposed to draw the mesh
            unsigned int GetVBO();
            unsigned int GetEBO();

            std::vector<Vertex>* GetVertices();
            std::vector<unsigned int>* GetIndices();

        private:
            //mesh vertices
            std::vector<Vertex> vertices;

            //order in which they connect to form triangles
            std::vector<unsigned int> indices;

            //vertex buffer and element buffer objects
            unsigned int VBO;
            unsigned int EBO;
    };

}

#endif // MESH_H
