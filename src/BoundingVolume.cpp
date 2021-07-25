#include "BoundingVolume.h"

void Engine::BoundingBox::Move(glm::vec3 offset)
{
    this->mins += offset;
    this->maxs += offset;
}

void Engine::BoundingBox::Move(glm::vec3 direction, float magnitude)
{
    this->mins += glm::normalize(direction) * magnitude;
    this->maxs += glm::normalize(direction) * magnitude;
}

void Engine::BoundingSphere::Move(glm::vec3 offset)
{
    this->center += offset;
    this->center += offset;
}

void Engine::BoundingSphere::Move(glm::vec3 direction, float magnitude)
{
    this->center += glm::normalize(direction) * magnitude;
    this->center += glm::normalize(direction) * magnitude;
}

bool Engine::BoundingBox::IntersectsWith(Engine::BoundingBox& other)
{
    //for an intersection to occur, it needs to happen on all three axes
    //for performance reasons, we split the check into three parts, if any of them return false, the whole function can imediatelly return false
    //we assume that multiple boxes can have perfectly aligned walls, which we count as an intersection, hence the (<=, >=) instead of (<, >)

    //x axis
    bool intersection = this->GetLeft() <= other.GetRight() && this->GetRight() >= other.GetLeft();

    if (!intersection)
        return false;
    else {
        //y axis
        intersection = this->GetBottom() <= other.GetTop() && this->GetTop() >= other.GetBottom();

        if (!intersection)
            return false;
        else {
            //z axis
            intersection = this->GetFront() <= other.GetBack() && this->GetBack() >= other.GetFront();

            if (!intersection)
                return false;
            else
                return true;
        }
    }
}

bool Engine::BoundingBox::IntersectsWith(Engine::BoundingSphere& sphere)
{
    float distance = glm::pow(sphere.radius, 2);

    //x axis
    if (sphere.center.x < this->mins.x)
        distance -= glm::pow(sphere.center.x - this->mins.x, 2);
    else
        if (sphere.center.x > this->maxs.x)
            distance -= glm::pow(sphere.center.x - this->maxs.x, 2);

    //y axis
    if (sphere.center.y < this->mins.y)
        distance -= glm::pow(sphere.center.y - this->mins.y, 2);
    else
        if (sphere.center.y > this->maxs.y)
            distance -= glm::pow(sphere.center.y - this->maxs.y, 2);

    //z axis
    if (sphere.center.z < this->mins.z)
        distance -= glm::pow(sphere.center.z - this->mins.z, 2);
    else
        if (sphere.center.z > this->maxs.z)
            distance -= glm::pow(sphere.center.z - this->maxs.z, 2);

    return distance > 0;
}

bool Engine::BoundingSphere::IntersectsWith(Engine::BoundingSphere& other)
{
    //check if the sum of radi is greater than the distance between the spheres
    return glm::distance(this->center, other.center) < (this->radius + other.radius);
}

bool Engine::BoundingSphere::IntersectsWith(Engine::BoundingBox& bBox)
{
    //just reuse the intersection algorithm from the bounding boxes perspective
    return bBox.IntersectsWith(*this);
}

float Engine::BoundingBox::GetBottom() {

    return this->mins.y;
}

float Engine::BoundingBox::GetLeft() {

    return this->mins.x;
}

float Engine::BoundingBox::GetFront() {

    return this->mins.z;
}

float Engine::BoundingBox::GetTop() {

    return this->maxs.y;
}

float Engine::BoundingBox::GetRight() {

    return this->maxs.x;
}

float Engine::BoundingBox::GetBack() {

    return this->maxs.z;
}

template<typename T>
Engine::BoundingNode<T>::BoundingNode()
{
    this->mins = {};
    this->maxs = {};

    this->depth = {};
    this->isLeaf = {};
    this->parent = {};
    this->siblings = {};
    this->children = {};
    this->data = {};
}

template<typename T>
Engine::BoundingNode<T>::~BoundingNode()
{
    if (this->parent) delete parent;
    if (this->siblings) delete siblings;
    if (this->children) delete children;
    if (this->data) delete data;
}

template<typename T>
Engine::BoundingNode<T>::BoundingNode(glm::vec3 mins, glm::vec3 maxs)
{
    this->mins = mins;
    this->maxs = maxs;

    this->depth = {};
    this->isLeaf = {};
    this->parent = {};
    this->siblings = {};
    this->children = {};
    this->data = {};
}

template<typename T>
void Engine::BoundingNode<T>::Subdivide(int maxDepth)
{
    //check if we've reached maximum depth
    if (this->depth != maxDepth)
    {
        //if not, spawn 8 children and raise them well
        this->children = new BoundingNode<T>[8];

        glm::vec3 halfSize = (this->maxs - this->mins) / 2;

        //setup the child coordinates relative to the proud parent's one
        //this probably could have been done in some black magic loop but I had too many coffees already
        //starting at this->mins location and going in clockwise direction

        //bottom near left
        this->children[0].mins = glm::vec3(this->mins);

        //bottom far left
        this->children[1].mins = glm::vec3(this->mins + halfSize.z);

        //bottom far right
        this->children[2].mins = glm::vec3(this->mins + halfSize.z + halfSize.x);

        //bottom near right
        this->children[3].mins = glm::vec3(this->mins + halfSize.x);

        //top near left
        this->children[4].mins = glm::vec3(this->mins + halfSize.y);

        //top far left
        this->children[5].mins = glm::vec3(this->mins + halfSize.z + halfSize.y);

        //top far right
        this->children[6].mins = glm::vec3(this->mins + halfSize);

        //top near right
        this->children[7].mins = glm::vec3(this->mins + halfSize.x + halfSize.y);

        //setup other children parameters
        for (size_t i = 0; i < 8; i++)
        {
            this->children[i].depth = this->depth - 1;
            this->children[i].isLeaf = false;
            this->children[i].parent = this;
            this->children[i].maxs = this->children[i].mins + halfSize;
            this->children[i].siblings = new BoundingNode<T>[7];

            //get siblings working
            for (size_t j = 0; j < 8; j++)
                if (this->children[i] != this->children[j])
                    this->children[i].siblings[j] = this->children[j];
        }

        //they grow up so fast, now they get to have children of their own
        for (size_t i = 0; i < 8; i++)
            this->children[i].Subdivide(this->depth);
    }
    else
    {
        this->isLeaf = true;
        this->children = {};
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
void Engine::OcTree<T>::Subdivide(int maxDepth)
{
    this->child->Subdivide(maxDepth);
}