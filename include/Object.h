#ifndef OBJECT_H
#define OBJECT_H

#define OBJL_INCLUDED

#include "string"
#include "memory"
#include "Model.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include "glfw3.h"


namespace Engine
{

    //base object class
    class Object
    {
        public:
            Object();
            Object(float x, float y, float z);

            Object(float tx, float ty, float tz,
                   float rx, float ry, float rz,
                   float sx, float sy, float sz);

            Object(glm::mat4 transform);
            Object(glm::vec3 position);
            Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

            float* GetPosition();
            float* GetRotation();
            float* GetScale();

            static Object* GetActiveObject();
            static void SetActiveObject(Object* object);
            bool IsMoving();

            std::string ToString();
            glm::mat4* GetTransform();
            Engine::Shader* GetShader();
            Engine::Model* GetModel();

            void SetTransform(glm::mat4 transformMatrix);
            void SetShader(const Engine::Shader& shader);
            void SetModel(const Engine::Model& model);

            void Draw(glm::mat4 view, glm::mat4 projection);

            void MoveRelative(float x, float y, float z);
            void MoveAbsolute(float x, float y, float z);

            void RotateRelative(float x, float y, float z);
            void RotateAbsolute(float x, float y, float z);

            void ScaleRelative(float x, float y, float z);
            void ScaleAbsolute(float x, float y, float z);

        protected:
            Model model;
            Shader shader;
            static Object* active;
            std::vector<Object*> children;
            glm::mat4 transform;
            bool isMoving;
    };

}
#endif // OBJECT_H
