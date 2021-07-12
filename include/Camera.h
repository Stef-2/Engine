#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glfw3.h"

namespace Engine
{

class Camera : public Object
{
    using Object::Object;

    public:
        void Setup(float speed, float nearClip, float farClip, float fov);
        ~Camera();

        glm::vec3 GetDirection();
        glm::mat4 GetView();
        glm::mat4 GetProjection();
        float GetSpeed();
        float GetNearClip();
        float GetFarClip();
        float GetFov();

        void SetDirection(glm::vec3 direction);
        void SetView(glm::mat4 view);
        void SetProjection(glm::mat4 projection);
        void SetSpeed(float speed);
        void SetNearClip(float nearClip);
        void SetFarClip(float farClip);
        void SetFov(float fov);


    private:
        glm::vec3 direction;
        glm::mat4 view;
        glm::mat4 projection;

        float speed;
        float nearClip;
        float farclip;
        float fov;
};

}
#endif // CAMERA_H
