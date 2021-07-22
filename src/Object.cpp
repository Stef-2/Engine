#include "Object.h"

Engine::Object* Engine::Object::active = nullptr;

Engine::Object::Object()
{
    this->children = {};
    this->isMoving = {};

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Engine::Object::Object(float x, float y, float z)
{
    this->children = {};
    this->isMoving = {};

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Engine::Object::Object(glm::vec3 position)
{
    this->children = {};
    this->isMoving = {};

    this->position = position;
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Engine::Object::Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->children = {};
    this->isMoving = {};

    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Engine::Object::Object(float tx, float ty, float tz,
                       float rx, float ry, float rz,
                       float sx, float sy, float sz)
{
    this->children = {};
    this->isMoving = {};

    this->position = glm::vec3(tx, ty, tz);
    this->rotation = glm::vec3(rx, ry, rz);
    this->scale = glm::vec3(sx, sy, sz);
}

glm::vec3 Engine::Object::GetPosition()
{
    return this->position;
}

glm::vec3 Engine::Object::GetRotation()
{
    return this->rotation;
}

glm::vec3 Engine::Object::GetScale()
{
    return this->scale;
}

void Engine::Object::MoveRelative(float x, float y, float z)
{
    this->position = this->position + glm::vec3(x, y, z);
}

void Engine::Object::MoveAbsolute(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
}

void Engine::Object::RotateRelative(float x, float y, float z)
{
    this->rotation = this->rotation + glm::vec3(x, y, z);
    if (this->rotation.x > 360) this->rotation.x -= 360;
    if (this->rotation.y > 360) this->rotation.y -= 360;
    if (this->rotation.z > 360) this->rotation.z -= 360;
}

void Engine::Object::RotateAbsolute(float x, float y, float z)
{
    this->rotation = glm::vec3(x, y, z);
}

void Engine::Object::ScaleRelative(float x, float y, float z)
{
    this->scale = this->scale + glm::vec3(x, y, z);
}

void Engine::Object::ScaleAbsolute(float x, float y, float z)
{
    this->scale = glm::vec3(x, y, z);
}

glm::mat4 Engine::Object::GetTransform()
{
    //build an identity matrix
    glm::mat4 transform = glm::mat4(1.0f);

    //build a quaternion out of our euler angles
    glm::quat quaternion = glm::quat(glm::radians(this->rotation));

    //the correct order of transformation is Scale, Rotate, Translate or SRT for short
    transform = glm::scale(transform, this->scale);
    transform = transform * glm::mat4_cast(quaternion);
    transform = glm::translate(transform, this->position);

    return transform;
}

bool Engine::Object::IsMoving()
{
    return this->isMoving;
}

Engine::Object* Engine::Object::GetActiveObject()
{
    return Engine::Object::active;
}

void Engine::Object::SetActiveObject(Engine::Object* object)
{
    Engine::Object::active = object;
}

std::string Engine::Object::ToString()
{
    std::string str = "Class name: ";
    str.append(typeid(this).name());
    str += ", With size: ";
    str.append(std::to_string(sizeof(this)));

    return str;
}
