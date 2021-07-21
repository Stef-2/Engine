#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"
#include "Actor.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glfw3.h"

namespace Engine
{

    //a camera class, inheriting transformation mechanisms from Engine::Object
    //provides the Vew() and Projection() attributes needed for rendering
    class Camera : public Object
    {
        using Object::Object;

        public:
            void Setup(float speed, float aspectRatio, float nearClip, float farClip, float fov);
            ~Camera();

            //view, extracted from camera's transform matrix
            glm::mat4 GetView();
            //camera direction, effectivelly just object rotation extracted from transform matrix
            glm::vec3 GetDirection();
            //projection matrix, combining field of view, aspect ratio, near and far clip planes
            glm::mat4 GetProjection();

            float GetSpeed();
            float GetNearClip();
            float GetFarClip();
            //field of view
            float GetFov();

            void SetUpDirection(glm::vec3 direction);
            void SetProjection(glm::mat4 projection);
            void SetSpeed(float speed);
            void SetNearClip(float nearClip);
            void SetFarClip(float farClip);
            void SetFov(float fov);
            void Draw(Engine::Object* actor);

        private:
            glm::vec3 upDirection;
            glm::mat4 projection;

            float speed;
            float nearClip;
            float farclip;
            float fov;
    };

}
#endif // CAMERA_H
