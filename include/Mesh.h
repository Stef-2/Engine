#ifndef MESH_H
#define MESH_H

#include "Shader.h"

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
        glm::vec3 Position;
        glm::vec2 TexCoords;
    };

    class Mesh
    {
        public:
            Mesh();
            Mesh(const char* filePath);
            ~Mesh();
            Mesh(const Mesh& other);
            Mesh& operator=(const Mesh& other);

            void Setup(const char* filePath);
            void Draw(Shader* shader);

        private:
            //mesh vertices
            std::vector<Vertex> vertexBufferr;

            //order in which they connect to form triangles
            std::vector<unsigned int> indices;

            //vertex buffer and element buffer objects
            unsigned int VBO;
            unsigned int EBO;

            //Shader shader;

    };

}

#endif // MESH_H
