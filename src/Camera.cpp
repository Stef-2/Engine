#include "Camera.h"


Engine::Camera::~Camera()
{
    //if (position != NULL) delete position;
    //if (rotation != NULL) delete rotation;
    //if (scale != NULL) delete scale;
}

void Engine::Camera::Setup(float speed, float aspectRatio, float nearClip, float farClip, float fov)
{
    this->speed = speed;
    this->nearClip = nearClip;
    this->farclip = farClip;
    this->fov = fov;

    this->projection = glm::perspective(fov, aspectRatio, nearClip, farClip);
    this->upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Engine::Camera::GetDirection()
{
    //camera direction is in essence the rotation component of its transform matrix, so we can just return that
    glm::vec3 direction = glm::vec3(this->GetRotation()[0], this->GetRotation()[1], this->GetRotation()[2]);

    return direction;
}

glm::mat4 Engine::Camera::GetView()
{
    //extract position from transform matrix
    glm::vec3 position = glm::vec3(this->GetPosition()[0], this->GetPosition()[1], this->GetPosition()[2]);
    //extract and normalize rotation from our transform matrix too, use that as direction
    glm::vec3 direction = glm::normalize(glm::vec3(this->GetRotation()[0], this->GetRotation()[1], this->GetRotation()[2]));
    //combine them with the Up vector into a view matrix
    glm::mat4 view = glm::lookAt(position, position + direction, this->upDirection);

    return view;
}

glm::mat4 Engine::Camera::GetProjection()
{
    return this->projection;
}

float Engine::Camera::GetSpeed()
{
    return this->speed;
}

float Engine::Camera::GetNearClip()
{
    return this->nearClip;
}

float Engine::Camera::GetFarClip()
{
    return this->farclip;
}

float Engine::Camera::GetFov()
{
    return this->fov;
}

void Engine::Camera::SetUpDirection(glm::vec3 direction)
{
    this->upDirection = direction;
}

void Engine::Camera::SetProjection(glm::mat4 projection)
{
    this->projection = projection;
}
void Engine::Camera::SetSpeed(float speed)
{
    this->speed = speed;
}

void Engine::Camera::SetNearClip(float nearClip)
{
    this->nearClip = nearClip;
}

void Engine::Camera::SetFarClip(float farClip)
{
    this->farclip = farClip;
}

void Engine::Camera::SetFov(float fov)
{
    this->fov = fov;
}

void Engine::Camera::Draw(Engine::Object* actor)
{
    //find the locations of uniform variables in the shader and assign transform matrices to them
    int modelLoc = glGetUniformLocation(static_cast<Engine::Actor*>(actor)->GetShader()->GetProgramID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->transform));

    //check if the of uniform variable was found
    if (modelLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of model uniform var was not found" << std::endl;
        return;
    }

    int viewLoc = glGetUniformLocation(static_cast<Engine::Actor*>(actor)->GetShader()->GetProgramID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(this->GetView()));

    //check if the of uniform variable was found
    if (viewLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of view uniform var was not found" << std::endl;
        return;
    }

    int projectionLoc = glGetUniformLocation(static_cast<Engine::Actor*>(actor)->GetShader()->GetProgramID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(this->GetProjection()));

    //check if the of uniform variable was found
    if (projectionLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of projection uniform var was not found" << std::endl;
        return;
    }

    //pass the draw call to the encapsulated model
    static_cast<Engine::Actor*>(actor)->GetModel()->Draw(static_cast<Engine::Actor*>(actor)->GetShader());
}
