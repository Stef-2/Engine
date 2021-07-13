#ifndef OBJECT_H
#define OBJECT_H

#define OBJL_INCLUDED

#include "string"
#include "memory"
#include "Model.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include "glfw3.h"


namespace Engine
{


class Object
{
    public:
        Object();
        Object(float* position);
        Object(float* position, float* rotation, float* scale);
        ~Object();
        Object(const Object& other);
        Object& operator=(const Object& other);

        float* GetPosition();
        float* GetRotation();
        float* GetScale();
        bool IsMoving();
        std::string ToString();
        glm::mat4 GetTransform();
        Engine::Shader GetShader();
        Engine::Model GetModel();
        void Draw(glm::mat4 view, glm::mat4 projection);
        void SetShader(Engine::Shader* shader);
        void SetModel(Engine::Model* model);

        void Transform(glm::mat4 transformMatrix);
        void UpdateTransform();
        void SetPosition(float* position);
        void SetRotation(float* rotation);
        void SetScale(float* scale);

        void MoveRelative(float* offset);
        void MoveAbsolute(float* location);

        void RotateRelative(float* offset);
        void RotateAbsolute(float* rotation);

        void ScaleRelative(float* offset);
        void ScaleAbsolute(float* location);


    protected:
        Model* model;
        Shader* shader;
        Object* children;
        glm::mat4 transform;
        float* position;
        float* rotation;
        float* scale;
        bool isMoving;
};

}
#endif // OBJECT_H
