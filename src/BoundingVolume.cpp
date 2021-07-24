#include "BoundingVolume.h"

void Engine::BoundingBox::Move(glm::vec3 offset)
{
    this->first += offset;
    this->second += offset;
}

void Engine::BoundingBox::Move(glm::vec3 direction, float magnitude)
{
    this->first += glm::normalize(direction) * magnitude;
    this->second += glm::normalize(direction) * magnitude;
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
    if (sphere.center.x < this->first.x)
        distance -= glm::pow(sphere.center.x - this->first.x, 2);
    else
        if (sphere.center.x > this->second.x)
            distance -= glm::pow(sphere.center.x - this->second.x, 2);

    //y axis
    if (sphere.center.y < this->first.y)
        distance -= glm::pow(sphere.center.y - this->first.y, 2);
    else
        if (sphere.center.y > this->second.y)
            distance -= glm::pow(sphere.center.y - this->second.y, 2);

    //z axis
    if (sphere.center.z < this->first.z)
        distance -= glm::pow(sphere.center.z - this->first.z, 2);
    else
        if (sphere.center.z > this->second.z)
            distance -= glm::pow(sphere.center.z - this->second.z, 2);

    return distance > 0;
}

bool Engine::BoundingSphere::IntersectsWith(Engine::BoundingSphere& other)
{
    //check if the sum of radi is greater than the distance between the spheres
    return glm::distance(this->center, other.center) < (this->radius + other.radius);
}

bool Engine::BoundingSphere::IntersectsWith(Engine::BoundingBox& bBox)
{
    //just reuse the algorithm from the bounding boxes perspective
    return bBox.IntersectsWith(*this);
}

inline float Engine::BoundingBox::GetBottom() {

    return this->first.y;
}

inline float Engine::BoundingBox::GetLeft() {

    return this->first.x;
}

inline float Engine::BoundingBox::GetFront() {

    return this->first.z;
}

inline float Engine::BoundingBox::GetTop() {

    return this->second.y;
}

inline float Engine::BoundingBox::GetRight() {

    return this->second.x;
}

inline float Engine::BoundingBox::GetBack() {

    return this->second.z;
}
