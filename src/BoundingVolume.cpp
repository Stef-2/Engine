#include "BoundingVolume.h"

void Engine::BoundingBox::Move(glm::vec3 offset)
{
    this->minimums += offset;
    this->maximums += offset;
}

Engine::Point Engine::BoundingBox::ClosestPoint(Engine::Point& point)
{
    Engine::Point result;

    // handle outside cases
    result.x = (point.x > this->maximums.x) ? this->maximums.x : point.x;
    result.x = (point.x < this->minimums.x) ? this->minimums.x : point.x;

    return result;
}

void Engine::BoundingBox::Move(glm::vec3 direction, float magnitude)
{
    this->minimums += glm::normalize(direction) * magnitude;
    this->maximums += glm::normalize(direction) * magnitude;
}

void Engine::BoundingSphere::Move(glm::vec3 offset)
{
    this->center += offset;
    this->center += offset;
}

float Engine::BoundingSphere::SurfaceArea()
{
    return 4 * glm::pi<float>() * glm::pow(this->radius, 2);
}

float Engine::BoundingSphere::Volume()
{
    return 4/3 * glm::pi<float>() * glm::pow(this->radius, 3); 
}

void Engine::BoundingSphere::Move(glm::vec3 direction, float magnitude)
{
    this->center += glm::normalize(direction) * magnitude;
    this->center += glm::normalize(direction) * magnitude;
}

float Engine::BoundingBox::GetBottom() {

    return this->minimums.y;
}

float Engine::BoundingBox::GetLeft() {

    return this->minimums.x;
}

float Engine::BoundingBox::GetFront() {

    return this->minimums.z;
}

float Engine::BoundingBox::GetTop() {

    return this->maximums.y;
}

float Engine::BoundingBox::GetRight() {

    return this->maximums.x;
}

float Engine::BoundingBox::GetBack() {

    return this->maximums.z;
}

void Engine::BoundingCapsule::Move(glm::vec3 offset)
{
    this->pointA += offset;
    this->pointB += offset;
}

void Engine::BoundingCapsule::Move(glm::vec3 normalizedDirection, float magnitude)
{
    this->pointA += glm::normalize(normalizedDirection) * magnitude;
    this->pointB += glm::normalize(normalizedDirection) * magnitude;
}
