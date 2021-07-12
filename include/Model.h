#ifndef MODEL_H
#define MODEL_H

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

namespace Engine
{

class Model
{
    public:
        Model() = delete;
        Model(const char* filePath);
        ~Model();
        Model(const Model& other);
        Model& operator=(const Model& other);
        void Draw(Shader* shader);

    private:
        float* vertexBuffer;    //mesh vertices
        unsigned int* indices;  //order in which they connect to form triangles

        //vertex buffer and element buffer objects
        unsigned int VBO;
        unsigned int EBO;

        //Shader shader;

};

}

#endif // MODEL_H
