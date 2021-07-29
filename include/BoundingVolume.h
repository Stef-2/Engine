#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#ifndef __glad_h_
#include "glad/glad.h"
#define __glad_h_
#endif

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "glfw3.h"

#include "memory"

namespace Engine
{
    //class Camera;

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

        //draws the bounding box in wireframe mode
        //not much use for this besides debugging

        //mins
        float GetBottom();
        float GetLeft();
        float GetFront();

        //maxs
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

    //a bounding box based struct meant to be used as a node in the ocTree
    //holds information about its relatives and enclosing data in additon to the inherited position info
    template<typename T>
    struct BoundingNode : Engine::BoundingBox
    {
        BoundingNode();
        ~BoundingNode();
        BoundingNode(glm::vec3 mins, glm::vec3 maxs);

        BoundingNode<T>* traverse(int depth);
        void Subdivide(int maxDepth);

        BoundingNode* parent;
        BoundingNode* siblings[7];
        BoundingNode* children[8];

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
        void Subdivide();

        BoundingNode<T>* Traverse(int depth);

        BoundingNode<T> child;
        unsigned int depth;
    };

    //templated classes need to have their member functions defined in the same header file, so I'm writing them here, below

    template<typename T>
    Engine::BoundingNode<T>::BoundingNode()
    {
        this->mins = {};
        this->maxs = {};

        this->depth = {};
        this->isLeaf = {};
        this->parent = {};
        //this->siblings = {};
        *this->children = {};
        this->data = {};
    }

    template<typename T>
    Engine::BoundingNode<T>::~BoundingNode()
    {
        //if (this->parent) delete parent;
        //if (this->siblings) delete siblings;
        //if (this->children) delete children;
        //if (this->data) delete data;
    }

    template<typename T>
    Engine::BoundingNode<T>::BoundingNode(glm::vec3 mins, glm::vec3 maxs)
    {
        this->mins = mins;
        this->maxs = maxs;

        this->depth = {};
        this->isLeaf = {};
        this->parent = nullptr;
        //this->siblings = nullptr;
        //this->children = {};
        this->data = {};
    }

    template<typename T>
    void Engine::BoundingNode<T>::Subdivide(int maxDepth)
    {
        //check if we've reached maximum recursion depth
        if (this->depth != maxDepth)
        {
            //if not, spawn 8 children and raise them well
            //setup children parameters
            for (size_t i = 0; i < 8; i++)
            {
                this->children[i] = new BoundingNode<T>;
                this->children[i]->depth = this->depth + 1;
                this->children[i]->isLeaf = false;
                this->children[i]->parent = this;

                //setup siblings, remember that we're not supposed to be our own sibling
                for (size_t j = 0; j < 8; j++)
                    if (j != i)
                        this->children[i]->siblings[j] = this->children[j];
            }

            //define half the size of the original bounding box
            glm::vec3 halfSize = (this->maxs - this->mins) / 2.0f;

            //break it down into offsets on all three axes
            glm::vec3 xOffset = glm::vec3(halfSize.x, 0.0f, 0.0f);
            glm::vec3 yOffset = glm::vec3(0.0f, halfSize.y, 0.0f);
            glm::vec3 zOffset = glm::vec3(0.0f, 0.0f, halfSize.z);

            //setup the child coordinates relative to the proud parent's one
            //starting at this->mins location and going in clockwise +Y direction
            //this probably could have been done in some black magic loop but I had too many coffees already

            //bottom near left
            this->children[0]->mins = glm::vec3(this->mins);

            //bottom far left
            this->children[1]->mins = glm::vec3(this->mins + zOffset);

            //bottom far right
            this->children[2]->mins = glm::vec3(this->mins + zOffset + xOffset);

            //bottom near right
            this->children[3]->mins = glm::vec3(this->mins + xOffset);

            //top near left
            this->children[4]->mins = glm::vec3(this->mins + yOffset);

            //top far left
            this->children[5]->mins = glm::vec3(this->mins + zOffset + yOffset);

            //top far right
            this->children[6]->mins = glm::vec3(this->mins + halfSize);

            //top near right
            this->children[7]->mins = glm::vec3(this->mins + xOffset + yOffset);

            //we haven't reached max depth, so subdivide further
            for (size_t i = 0; i < 8; i++) {
                //set the maxs while we're conveniently in a loop
                this->children[i]->maxs = this->children[i]->mins + halfSize;
                this->children[i]->Subdivide(maxDepth);
            }
        }
        else {
            //we've reached the maximum recursion depth
            this->isLeaf = true;
            //this->children = {};
        }
    }

    template<typename T>
    Engine::OcTree<T>::OcTree()
    {
        this->child = {};
        this->depth = {};
    }

    template<typename T>
    Engine::OcTree<T>::OcTree(int maxDepth)
    {
        this->child = {};
        this->depth = maxDepth;
    }

    template<typename T>
    void Engine::OcTree<T>::Subdivide()
    {
        this->child.Subdivide(this->depth);
    }

    template<typename T>
    Engine::BoundingNode<T>* Engine::OcTree<T>::Traverse(int depth)
    {

    }

}
#endif

