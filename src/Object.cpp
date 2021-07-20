#include "Object.h"

Engine::Object* Engine::Object::active = nullptr;

Engine::Object::Object()
{
    this->children = {};
    this->isMoving = {};

    this->transform = glm::mat4(1.0f);
}

Engine::Object::Object(float x, float y, float z)
{
    this->children = {};
    this->isMoving = {};

    //build an identity matrix
    this->transform = glm::mat4(1.0f);
    //move to the required position
    this->transform = glm::translate(this->transform, glm::vec3(x, y, z));
}

Engine::Object::Object(glm::mat4 transformMatrix)
{
    this->children = {};
    this->isMoving = {};

    this->transform = transformMatrix;
}

Engine::Object::Object(glm::vec3 position)
{
    this->children = {};
    this->isMoving = {};

    //build an identity matrix
    this->transform = glm::mat4(1.0f);
    //move to the required position
    this->transform = glm::translate(this->transform, position);
}

Engine::Object::Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->children = {};
    this->isMoving = {};

    //build an identity matrix
    this->transform = glm::mat4(1.0f);

    //build a quaternion out of the euler rotations
    glm::quat quaternion(glm::radians(rotation));

    //the correct order of matrix transformation is Scale, Rotate, Translate or SRT for short
    this->transform = glm::scale(this->transform, scale);
    this->transform = this->transform * glm::mat4_cast(quaternion);
    this->transform = glm::translate(this->transform, position);
}

Engine::Object::Object(float tx, float ty, float tz,
                       float rx, float ry, float rz,
                       float sx, float sy, float sz)
{
    this->children = {};
    this->isMoving = {};

    //build a quaternion out of the euler rotations
    glm::quat quaternion(glm::radians(glm::vec3(rx, ry, rz)));
    
    //build an identity matrix
    this->transform = glm::mat4(1.0f);
    
    //the correct order of matrix transformation is Scale, Rotate, Translate or SRT for short
    this->transform = glm::scale(this->transform, glm::vec3(sx, sy, sz));
    this->transform = this->transform * glm::mat4_cast(quaternion);
    this->transform = glm::translate(this->transform, glm::vec3(tx, ty, tz));
}

float* Engine::Object::GetPosition()
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;

    //decompose the transform matrix into components so we can extract the needed data
    glm::decompose(this->transform, scale, rotation, position, skew, perspective);

    return glm::value_ptr(position);
}

float* Engine::Object::GetRotation()
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;
    
    //decompose the transform matrix into components so we can extract the needed data
    glm::decompose(this->transform, scale, rotation, position, skew, perspective);

    return glm::value_ptr(rotation);
}

float* Engine::Object::GetScale()
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;

    //decompose the transform matrix into components so we can extract the needed data
    glm::decompose(this->transform, scale, rotation, position, skew, perspective);

    return glm::value_ptr(scale);
}

void Engine::Object::MoveRelative(float x, float y, float z)
{
    glm::translate(this->transform, glm::vec3(x, y, z));
}

void Engine::Object::MoveAbsolute(float x, float y, float z)
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;

    //decompose the transform matrix into components so we can extract the needed data
    glm::decompose(this->transform, scale, rotation, position, skew, perspective);

    //revert the transform matrix into an identity one
    this->transform = glm::mat4(1.0f);

    //rebuild the matrix using the extracted plus input values
    //the correct order of matrix transformation is Scale, Rotate, Translate or SRT for short
    this->transform = glm::scale(this->transform, scale);
    this->transform = this->transform * glm::mat4_cast(rotation);
    this->transform = glm::translate(this->transform, glm::vec3(x, y, z));
}

void Engine::Object::RotateRelative(float x, float y, float z)
{
    //build a quaternion out of the euler rotations
    glm::quat quaternion(glm::radians(glm::vec3(x, y, z)));

    //multiply the transform matrix by the quaternion casted into a 4x4 matrix to perform the rotation
    this->transform = this->transform * glm::mat4_cast(quaternion);
}

void Engine::Object::RotateAbsolute(float x, float y, float z)
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;

    //decompose the transform matrix into components so we can extract the needed data
    glm::decompose(this->transform, scale, rotation, position, skew, perspective);

    //revert the transform matrix into an identity one
    this->transform = glm::mat4(1.0f);

    //build a quaternion out of the euler rotations
    glm::quat quaternion(glm::radians(glm::vec3(x, y, z)));

    //rebuild the matrix using the extracted plus input values
    //the correct order of matrix transformation is Scale, Rotate, Translate or SRT for short
    this->transform = glm::scale(this->transform, scale);
    this->transform = this->transform * glm::mat4_cast(quaternion);
    this->transform = glm::translate(this->transform, position);
}

void Engine::Object::ScaleRelative(float x, float y, float z)
{
    this->transform = glm::scale(this->transform, glm::vec3(x, y, z));
}

void Engine::Object::ScaleAbsolute(float x, float y, float z)
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;

    //decompose the transform matrix into components so we can extract the needed data
    glm::decompose(this->transform, scale, rotation, position, skew, perspective);

    //revert the transform matrix into an identity one
    this->transform = glm::mat4(1.0f);

    //rebuild the matrix using the extracted plus input values
    //the correct order of matrix transformation is Scale, Rotate, Translate or SRT for short
    this->transform = glm::scale(this->transform, glm::vec3(x, y, z));
    this->transform = this->transform * glm::mat4_cast(rotation);
    this->transform = glm::translate(this->transform, position);
}

void Engine::Object::SetTransform(glm::mat4 transformMatrix)
{
    this->transform = transformMatrix;
}

glm::mat4* Engine::Object::GetTransform()
{
    return &this->transform;
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
