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

void Engine::Actor::Draw(glm::mat4 view, glm::mat4 projection)
{
    //find the locations of uniform variables in the shader and assign transform matrices to them

    int modelLoc = glGetUniformLocation(this->shader.GetProgramID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->GetTransform()));

    //check if the of uniform variable was found
    if (modelLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of model uniform var was not found" << std::endl;
        return;
    }

    int viewLoc = glGetUniformLocation(this->shader.GetProgramID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    //check if the of uniform variable was found
    if (viewLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of view uniform var was not found" << std::endl;
        return;
    }

    int projectionLoc = glGetUniformLocation(this->shader.GetProgramID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //check if the of uniform variable was found
    if (projectionLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of projection uniform var was not found" << std::endl;
        return;
    }

    //pass the draw call to the encapsulated model
    model.Draw(&this->shader);
}

void Engine::Actor::Draw(Engine::Camera* camera)
{
    //find the locations of uniform variables in the shader and assign transform matrices to them

    glUniformMatrix4fv(this->shader.GetAttributeLocation(Engine::Shader::ShaderAttribute::MODEL_LOCATION), 1, GL_FALSE, glm::value_ptr(this->GetTransform()));

    glUniformMatrix4fv(this->shader.GetAttributeLocation(Engine::Shader::ShaderAttribute::VIEW_LOCATION), 1, GL_FALSE, glm::value_ptr(camera->GetView()));

    glUniformMatrix4fv(this->shader.GetAttributeLocation(Engine::Shader::ShaderAttribute::PROJECTION_LOCATION), 1, GL_FALSE, glm::value_ptr(camera->GetProjection()));

    //pass the draw call to the encapsulated model
    model.Draw(&this->shader);
}