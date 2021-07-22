#include "Actor.h"

void Engine::Actor::SetShader(const Engine::Shader& shader)
{
    this->shader = shader;
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
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->transform));

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

    int modelLoc = glGetUniformLocation(this->shader.GetProgramID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->transform));

    //check if the of uniform variable was found
    if (modelLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of model uniform var was not found" << std::endl;
        return;
    }

    int viewLoc = glGetUniformLocation(this->shader.GetProgramID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetView()));

    //check if the of uniform variable was found
    if (viewLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of view uniform var was not found" << std::endl;
        return;
    }

    int projectionLoc = glGetUniformLocation(this->shader.GetProgramID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->GetView()));

    //check if the of uniform variable was found
    if (projectionLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of projection uniform var was not found" << std::endl;
        return;
    }

    //pass the draw call to the encapsulated model
    model.Draw(&this->shader);
}