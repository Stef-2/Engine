#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glfw3.h"

#include "vector"

namespace Engine
{

    // a camera class, inheriting transformation mechanisms from Engine::Object
    // provides the Vew() and Projection() matrices needed for rendering
    // provides frustum clipping planes, needed for frustum culling
    class Camera : public Engine::Object
    {
        using Engine::Object::Object;

        public:
            void Setup(float speed, float aspectRatio, float nearClip, float farClip, float fov);

            // view, built from camera's transformations and the Up direction
            glm::mat4 GetView() const;
            // projection matrix, combining field of view, aspect ratio, near and far clip planes
            glm::mat4 GetProjection() const;

            // camera's forward, normalized direction vector; effectivelly this is just it's rotation
            glm::vec3 GetForwardDirection();

            // a normalized vector that determines the "Up" direction; usually positive Y axis (0.0, 1.0, 0.0)
            glm::vec3 GetUpDirection();

            // camera's right, normalized direction vector; cross product of Forward and Up vectors
            glm::vec3 GetRightDirection();

            // camera's view frustum clipping planes, to be used for frustum culling by the renderer
            std::vector<glm::vec4> GetFrustumPlanes() const;
            

            float GetSpeed();
            float GetAspectRatio();
            float GetNearClip();
            float GetFarClip();
            float GetFov();

            // camera's "Up" direction, usually positive direction of Y axis
            void SetUpDirection(glm::vec3 direction);

            // set and use an arbitrary projection matrix, any calls for UpdateProjection() made afterwards will invalidate this
            void SetProjection(glm::mat4 projection);

            // updates the projection matrix, should be called every time any of the following parameters change:
            // <fov, aspect ratio, near clip or far clip>, all functions that changes these already call this function
            void UpdateProjection();

            // speed expressed in arbitrary world units, to be used as a multiplier for movement
            void SetSpeed(float speed);

            // ratio of camera's frustum width and height dimensions
            // if the desired ratio is the same as rendering window's, it can be acquired from Engine::Window::GetAspectRatio()
            void SetAspectRatio(float aspectRatio);

            // elements closer than this value are not rendered
            void SetNearClip(float nearClip);

            // elements further than this value are not rendered
            void SetFarClip(float farClip);

            // field of view, expressed in degrees
            // usually 45 to 90 degrees
            void SetFov(float fov);

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
#endif //  CAMERA_H
