#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"
#include "Actor.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glfw3.h"

#include "vector"

namespace Engine
{

    //forward declaration of Engine::Actor
    //so it can be referenced by the draw function without causing a cyclic dependacy
    class Actor;

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
            //camera direction vector, effectivelly just object rotation extracted from transform matrix
            glm::vec3 GetDirection();
            //projection matrix, combining field of view, aspect ratio, near and far clip planes
            glm::mat4 GetProjection();

            float GetSpeed();
            float GetAspectRatio();
            float GetNearClip();
            float GetFarClip();

            //field of view, expressed in degrees
            //usually 45 to 90 degrees
            float GetFov();

            //world "Up" direction, usually positive direction of Y axis
            void SetUpDirection(glm::vec3 direction);
            void SetProjection(glm::mat4 projection);
            void SetProjection();
            //speed expressed in arbitrary world units, to be used as multiplier for movement
            void SetSpeed(float speed);
            void SetAspectRatio(float aspectRatio);
            void SetNearClip(float nearClip);
            void SetFarClip(float farClip);
            void SetFov(float fov);
            //use this camera to draw actor(s)
            void Draw(std::vector<Engine::Actor*> actors);
            void Draw(Engine::Actor* actor);

        private:
            glm::vec3 upDirection;
            glm::mat4 projection;

            float speed;
            float aspectRatio;
            float nearClip;
            float farclip;
            float fov;
    };

}
#endif // CAMERA_H
