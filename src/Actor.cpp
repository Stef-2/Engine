#include "Actor.h"

void Engine::Actor::SetShader(const Engine::ShaderProgram& shader)
{
    this->shader.reset();
    this->shader = std::make_shared<Engine::ShaderProgram>(shader);
}

void Engine::Actor::SetShader(const Engine::ShaderProgram* shader)
{
    this->shader.reset();
    this->shader = std::make_shared<Engine::ShaderProgram>(*shader);
}

void Engine::Actor::SetShader(const std::shared_ptr<Engine::ShaderProgram>& shader)
{
    this->shader.reset(shader.get());
}

void Engine::Actor::MoveRelative(glm::vec3 direction, float intensity)
{
    this->position = this->position + (intensity * glm::normalize(direction));

    // move the bounding box too
    this->model->GetBoundingBox().minimums += (intensity * glm::normalize(direction));
    this->model->GetBoundingBox().maximums += (intensity * glm::normalize(direction));
}

void Engine::Actor::MoveRelative(float x, float y, float z)
{
    this->position = this->position + glm::vec3(x, y, z);

    // move the bounding box too
    this->model->GetBoundingBox().minimums += glm::vec3(x, y, z);
    this->model->GetBoundingBox().maximums += glm::vec3(x, y, z);
}

void Engine::Actor::MoveAbsolute(float x, float y, float z)
{
    float distance = glm::distance(this->position, glm::vec3(x, y, z));
    this->position = glm::vec3(x, y, z);

    // move the bounding box too
    this->model->GetBoundingBox().minimums -= distance;
    this->model->GetBoundingBox().maximums -= distance;
}

Engine::ShaderProgram& Engine::Actor::GetShader()
{
    return *this->shader;
}

void Engine::Actor::SetModel(const Engine::Model& model)
{
    this->model = model;
}

Engine::Model& Engine::Actor::GetModel()
{
    return *this->model;
}