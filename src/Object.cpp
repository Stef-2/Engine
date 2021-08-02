#include "Object.h"

Engine::Object* Engine::Object::active = nullptr;

Engine::Object::Object()
{
    this->children = {};
    this->isMoving = {};

    this->orientation = {};
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Engine::Object::Object(float x, float y, float z)
{
    this->children = {};
    this->isMoving = {};

    this->orientation = {};
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Engine::Object::Object(glm::vec3 position)
{
    this->children = {};
    this->isMoving = {};

    this->orientation = {};
    this->position = position;
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Engine::Object::Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->children = {};
    this->isMoving = {};

    this->orientation = glm::quat(glm::radians(rotation));
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

    this->orientation = glm::quat(glm::radians(glm::vec3(rx, ry, rz)));
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

glm::quat Engine::Object::GetOrientation()
{
    return this->orientation;
}

void Engine::Object::SetOrientation(glm::quat quaternion)
{
    this->orientation = quaternion;
}

void Engine::Object::MoveRelative(glm::vec3 direction, float intensity)
{
    this->position = this->position + (intensity * glm::normalize(direction));
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
    //construct quaternions for each of the three axis rotations
    glm::quat pitch = glm::angleAxis(glm::radians(x), glm::vec3(1, 0, 0));
    glm::quat yaw = glm::angleAxis(glm::radians(y), glm::vec3(0, 1, 0));
    glm::quat roll = glm::angleAxis(glm::radians(z), glm::vec3(0, 0, 1));

    //due to the bizarre way multiple quaternion multiplications work -
    //we have to "sandwich" our orientation between these 3 incoming ones:
    //multiply them strictly in alternating order to prevent stray rotations from creeping in
    this->orientation = glm::normalize(pitch) * this->orientation;
    this->orientation = this->orientation * glm::normalize(yaw);
    this->orientation = glm::normalize(roll) * this->orientation;

    //keep track of regular euler rotations in case we need to print something we can actually understand
    this->rotation = this->rotation + glm::vec3(x, y, z);
    
    //keep them in 0 - 360 range to prevent overflows
    if (this->rotation.x > 360)
        this->rotation.x -= 360;
    if (this->rotation.y > 360)
        this->rotation.y -= 360;
    if (this->rotation.z > 360)
        this->rotation.z -= 360;

    if (this->rotation.x < -360)
        this->rotation.x += 360;
    if (this->rotation.y < -360)
        this->rotation.y += 360;
    if (this->rotation.z < -360)
        this->rotation.z += 360;
}

void Engine::Object::RotateAbsolute(float x, float y, float z)
{
    glm::quat quaternion = glm::quat(glm::radians(glm::vec3(x, y, z)));
    this->orientation = quaternion;
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
    transform = transform * glm::mat4_cast(this->orientation);
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
