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
