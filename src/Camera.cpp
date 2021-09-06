#include "Camera.h"

void Engine::Camera::Setup(double speed, double aspectRatio, double nearClip, double farClip, double fov)
{
    this->speed = speed;
    this->aspectRatio = aspectRatio;
    this->nearClip = nearClip;
    this->farclip = farClip;
    this->fov = fov;

    this->UpdateProjection();
    
    this->upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Engine::Camera::GetForwardDirection()
{
    // we can extract our forward looking direction from the view matrix
    glm::mat4 view = this->GetView();
    glm::vec3 direction = glm::vec3(-view[0][2], -view[1][2], -view[2][2]);
    
    return direction;
}

glm::vec3 Engine::Camera::GetUpDirection()
{
    return glm::normalize(this->upDirection);
}

glm::vec3 Engine::Camera::GetRightDirection()
{
    return glm::normalize(glm::cross(this->GetForwardDirection(), this->GetUpDirection()));
}

glm::mat4 Engine::Camera::GetView() const
{
    // we're way too cool to construct a view matrix using LookAt(), so, 
    // convert our orientation quaternion into a rotation matix
    glm::mat4 direction = glm::mat4_cast(this->orientation);
    
    // construct a translation matrix from our position
    glm::mat4 position = glm::mat4(1.0f);
    position = glm::translate(position, -this->position);

    // multiply the two to create a view matrix B-)
    return direction * position;
}

std::vector<glm::vec4> Engine::Camera::GetFrustumPlanes() const
{
    // full camera matrix from which we'll extract the planes
    glm::mat4 matrix = this->projection * this->GetView();

    // OpenGL uses collumn-major matrix orientation, so we need to transpose it before extraction
    matrix = glm::transpose(matrix);

    glm::vec4 left {matrix[3] + matrix[0]};
    glm::vec4 right {matrix[3] - matrix[0]};

    glm::vec4 top {matrix[3] - matrix[1]};
    glm::vec4 bottom {matrix[3] + matrix[1]};

    glm::vec4 near {matrix[3] + matrix[2]};
    glm::vec4 far {matrix[3] - matrix[2]};

    // array of planes to fill in
    // normalizing them means that the first three values of each plane will be their respective normal
    std::vector<glm::vec4> planes = {glm::normalize(left), glm::normalize(right), glm::normalize(top),
                           glm::normalize(bottom), glm::normalize(near), glm::normalize(far)};
    
    return planes;
}

glm::mat4 Engine::Camera::GetProjection() const
{
    return this->projection;
}

double Engine::Camera::GetSpeed()
{
    return this->speed;
}

double Engine::Camera::GetAspectRatio()
{
    return this->aspectRatio;
}

double Engine::Camera::GetNearClip()
{
    return this->nearClip;
}

double Engine::Camera::GetFarClip()
{
    return this->farclip;
}

double Engine::Camera::GetFov()
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

void Engine::Camera::UpdateProjection()
{
    this->projection = glm::perspective(this->fov, this->aspectRatio, this->nearClip, this->farclip);
}

void Engine::Camera::SetSpeed(double speed)
{
    this->speed = speed;
}

void Engine::Camera::SetAspectRatio(double aspectRatio)
{
    this->aspectRatio = aspectRatio;
    this->UpdateProjection();
}

void Engine::Camera::SetNearClip(double nearClip)
{
    this->nearClip = nearClip;
    this->UpdateProjection();
}

void Engine::Camera::SetFarClip(double farClip)
{
    this->farclip = farClip;
    this->UpdateProjection();
}

void Engine::Camera::SetFov(double fov)
{
    this->fov = fov;
    this->UpdateProjection();
}