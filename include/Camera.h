#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"
#include "Actor.h"
#include "Skybox.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BoundingVolume.h"
#include "glfw3.h"

#include "vector"

namespace Engine
{

    //forward declaration of Engine::Actor
    //so it can be referenced by the draw function without causing a cyclic dependacy
    class Actor;
    class Skybox;

    //a camera class, inheriting transformation mechanisms from Engine::Object
    //provides the Vew() and Projection() matrices needed for rendering of Engine::Actor(s)
    //alternatively, it can initiate rendering itself, taking Engine::Actor(s) as input(s)
    //both methods can be used interchangeably
    class Camera : public Engine::Object
    {
        using Engine::Object::Object;

        public:
            void Setup(float speed, float aspectRatio, float nearClip, float farClip, float fov);
            ~Camera();

            //view, built from camera's transformations and the Up direction
            glm::mat4 GetView();
            //projection matrix, combining field of view, aspect ratio, near and far clip planes
            glm::mat4 GetProjection();

            //camera's forward, normalized direction vector; effectivelly this is just it's rotation
            glm::vec3 GetForwardDirection();
            //a normalized vector that determines the "Up" direction; usually positive Y axis (0.0, 1.0, 0.0)
            glm::vec3 GetUpDirection();
            //camera's right, normalized direction vector; cross product of Forward and Up vectors
            glm::vec3 GetRightDirection();
            //camera's view frustum clipping planes, to be used for frustum culling by the renderer
            glm::vec4* GetFrustumPlanes();
            

            float GetSpeed();
            float GetAspectRatio();
            float GetNearClip();
            float GetFarClip();
            float GetFov();

            //camera's "Up" direction, usually positive direction of Y axis
            void SetUpDirection(glm::vec3 direction);
            //set and use an arbitrary projection matrix, any calls for UpdateProjection() made afterwards will invalidate this
            void SetProjection(glm::mat4 projection);
            //updates the projection matrix, should be called every time any of the following parameters change:
            //<fov, aspect ratio, near clip or far clip>, all functions that changes these already call this function
            void UpdateProjection();
            //speed expressed in arbitrary world units, to be used as a multiplier for movement
            void SetSpeed(float speed);
            //ratio of camera's frustum width and height dimensions
            //if the desired ratio is the same as OpenGL's rendering window, it can be acquired from Engine::Window::GetAspectRatio()
            void SetAspectRatio(float aspectRatio);
            //elements closer than this value are not rendered
            void SetNearClip(float nearClip);
            //elements further than this value are not rendered
            void SetFarClip(float farClip);
            //field of view, expressed in degrees
            //usually 45 to 90 degrees
            void SetFov(float fov);

            //use this camera to draw actor(s)
            void Draw(std::vector<Engine::Actor*> actors);
            void Draw(Engine::Actor* actor);

            //use this camera to draw Bouding Volumes
            template<typename T>
            void Draw(Engine::OcTree<T>* tree);
            template<typename T>
            void RecursiveDraw(Engine::BoundingNode<T>* boundingNode);
            void Draw(Engine::BoundingBox* boundingBox);

            //use this camera to draw a skybox
            void Draw(Engine::Skybox* skybox);

        private:
            glm::vec3 upDirection;
            glm::mat4 projection;

            float speed;
            float aspectRatio;
            float nearClip;
            float farclip;
            float fov;
    };

    //bounding volumes are templeted types, we have to define their draw methods here

    template<typename T>
    void Engine::Camera::RecursiveDraw(Engine::BoundingNode<T>* boundingNode)
    {
        //draw ourselves
        Draw(boundingNode);

        //if we're not a leaf, pass the draw call to the kids
        if (!boundingNode->isLeaf)
            for (short i = 0; i < 8; i++)
                RecursiveDraw(boundingNode->children[i]);
    }

    template<typename T>
    void Engine::Camera::Draw(Engine::OcTree<T>* tree)
    {
        RecursiveDraw(&tree->child);
    }
}
#endif // CAMERA_H
