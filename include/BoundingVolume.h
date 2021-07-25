#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "glm/glm.hpp"

#include "memory"

namespace Engine
{
    //forward declaration of BoudingSphere so BoundingBox can reference it
    struct BoundingSphere;

    //axis aligned bounding box, fully encloses a mesh
    //we're defining a bounding box using two diagonal corners
    //           o-------o <-maxs
    //          /|      /|
    //         o-+-----o |
    //         | o-----+-o
    //         |/      |/
    //  mins-> o-------o
    struct BoundingBox
    {
        //intersection checks
        bool IntersectsWith(Engine::BoundingBox& other);
        bool IntersectsWith(Engine::BoundingSphere& sphere);

        void Move(glm::vec3 offset);
        void Move(glm::vec3 direction, float magnitude);

        float GetBottom();
        float GetLeft();
        float GetFront();
        float GetTop();
        float GetRight();
        float GetBack();

        //bottom, left, front
        glm::vec3 mins;
        //top, right, back
        glm::vec3 maxs;
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

    //a struct meant to be used as a node in the ocTree
    //holds information about its relatives and enclosing data in additon to the inherited position info
    template<typename T>
    struct BoundingNode : Engine::BoundingBox
    {
        BoundingNode();
        ~BoundingNode();
        BoundingNode(glm::vec3 mins, glm::vec3 maxs);

        void Subdivide(int maxDepth);
        
        BoundingBox* parent;
        BoundingBox* siblings;
        BoundingBox* children;

        unsigned int depth;
        bool isLeaf;
        T* data;
    };

    //Octree class, used to recursively subdivide 3D space into 8 equally sized BoundingBoxes (BoundingNodes)
    template<typename T>
    class OcTree
    {
    public:
        OcTree();
        OcTree(int depth);
        void Subdivide(int maxDepth);

        BoundingNode<T> child;
        unsigned int depth;
    };
}
#endif // VERTEXBUFFER_H

