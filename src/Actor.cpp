#include "Actor.h"

void Engine::Actor::SetShader(const Engine::Shader& shader)
{
    this->shader = shader;
}

void Engine::Actor::MoveRelative(glm::vec3 direction, float intensity)
{
    this->position = this->position + (intensity * glm::normalize(direction));

    //move the bounding box too
    this->model.GetBoundingBox()->mins += (intensity * glm::normalize(direction));
    this->model.GetBoundingBox()->maxs += (intensity * glm::normalize(direction));
}

void Engine::Actor::MoveRelative(float x, float y, float z)
{
    this->position = this->position + glm::vec3(x, y, z);

    //move the bounding box too
    this->model.GetBoundingBox()->mins += glm::vec3(x, y, z);
    this->model.GetBoundingBox()->maxs += glm::vec3(x, y, z);
}

void Engine::Actor::MoveAbsolute(float x, float y, float z)
{
    float distance = glm::distance(this->position, glm::vec3(x, y, z));
    this->position = glm::vec3(x, y, z);

    //move the bounding box too
    this->model.GetBoundingBox()->mins -= distance;
    this->model.GetBoundingBox()->maxs -= distance;
}

Engine::Shader* Engine::Actor::GetShader()
{
    return &this->shader;
}

void Engine::Actor::SetModel(const Engine::Model& model)
{
    this->model = model;
}

Engine::Model* Engine::Actor::GetModel()
{
    return &this->model;
}