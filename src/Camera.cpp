#include "Camera.h"


Engine::Camera::~Camera()
{
    //if (position != NULL) delete position;
    //if (rotation != NULL) delete rotation;
    //if (scale != NULL) delete scale;
}

void Engine::Camera::Setup(float speed, float nearClip, float farClip, float fov)
{
    this->speed = speed;
    this->nearClip = nearClip;
    this->farclip = farClip;
    this->fov = fov;

    this->direction = glm::vec3(1.0f);
    this->projection = glm::mat4(1.0f);
    this->view = glm::mat4(1.0f);
}

glm::vec3 Engine::Camera::GetDirection()
{
    return this->direction;
}

glm::mat4 Engine::Camera::GetView()
{
    return this->view;
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

void Engine::Camera::SetDirection(glm::vec3 direction)
{
    this->direction = direction;
}

void Engine::Camera::SetView(glm::mat4 view)
{
    this->view = view;
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
