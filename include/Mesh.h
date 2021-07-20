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

        //texture coordinates
        glm::vec2 uv;
    };

    //we're defining a bounding box using two diagonal corners
    //           o-------o <-second
    //          /|      /|
    //         o-+-----o |
    //         | o-----+-o
    //         |/      |/
    // first-> o-------o
    struct BoundingBox
    {
    public:
        float GetBottom();
        float GetLeft();
        float GetFront();
        float GetTop();
        float GetRight();
        float GetBack();

    private:
        //bottom, left, front
        glm::vec3 first;

        //top, right, back
        glm::vec3 second;
    };

    class Mesh
    {
        public:
            Mesh();
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
            //~Mesh();
            //Mesh(const Mesh& other);
            //Mesh& operator=(const Mesh& other);

            void Setup();
            void Draw(Shader* shader, Material* material);

            unsigned int GetVBO();
            unsigned int GetEBO();

            std::vector<Vertex>* GetVertices();
            std::vector<unsigned int>* GetIndices();
            BoundingBox GetBoundingBox();

        private:
            //mesh vertices
            std::vector<Vertex> vertices;

            //order in which they connect to form triangles
            std::vector<unsigned int> indices;

            BoundingBox boundingBox;

            //vertex buffer and element buffer objects
            unsigned int VBO;
            unsigned int EBO;

            //Shader shader;

    };

}

#endif // MESH_H
