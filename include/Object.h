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

    // base object class, containing transformation
    // and children data
    class Object
    {
        public:
            Object();
            Object(float x, float y, float z);

            Object(float tx, float ty, float tz,
                   float rx, float ry, float rz,
                   float sx, float sy, float sz);

            Object(glm::vec3 position);
            Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

            glm::quat GetOrientation();
            glm::vec3 GetPosition();
            glm::vec3 GetRotation();
            glm::vec3 GetScale();

            static Object& GetActiveObject();
            static void SetActiveObject(Object& object);
            bool IsMoving();

            std::string ToString();
            glm::mat4 GetTransform();

            void SetOrientation(glm::quat quaternion);

            virtual void MoveRelative(glm::vec3 direction, float intensity);
            virtual void MoveRelative(float x, float y, float z);
            virtual void MoveAbsolute(float x, float y, float z);

            void RotateRelative(float x, float y, float z);
            void RotateAbsolute(float x, float y, float z);

            void ScaleRelative(float x, float y, float z);
            void ScaleAbsolute(float x, float y, float z);

        protected:
            static Object* active;
            std::vector<Object*> children;
            glm::quat orientation;
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;
            bool isMoving;
    };

}
#endif //  OBJECT_H
