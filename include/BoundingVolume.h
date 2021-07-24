#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "glm/glm.hpp"

namespace Engine
{
    struct BoundingSphere;

    //axis aligned bounding box, fully encloses a mesh
    //we're defining a bounding box using two diagonal corners
    //           o-------o <-second
    //          /|      /|
    //         o-+-----o |
    //         | o-----+-o
    //         |/      |/
    // first-> o-------o
    struct BoundingBox
    {
        //intersection checks
        bool IntersectsWith(Engine::BoundingBox& other);
        bool IntersectsWith(Engine::BoundingSphere& sphere);

        void Move(glm::vec3 offset);
        void Move(glm::vec3 direction, float magnitude);

        inline float GetBottom();
        inline float GetLeft();
        inline float GetFront();
        inline float GetTop();
        inline float GetRight();
        inline float GetBack();

        //bottom, left, front
        glm::vec3 first;
        //top, right, back
        glm::vec3 second;
    };

    //bounding sphere, fully encloses a mesh
    struct BoundingSphere
    {
        //intersection checks
        bool IntersectsWith(Engine::BoundingBox& bBox);
        bool IntersectsWith(Engine::BoundingSphere& other);

        void Move(glm::vec3 offset);
        void Move(glm::vec3 direction, float magnitude);

        glm::vec3 center;
        float radius;
    };
}
#endif // VERTEXBUFFER_H

