#include "Object.h"

Engine::Object::Object()
{
    this->position = new float[3] {0.0f, 0.0f, 0.0f};
    this->rotation = new float[3] {0.0f, 0.0f, 0.0f};
    this->scale = new float[3] {1.0f, 1.0f, 1.0f};

    this->UpdateTransform();
    this->transform = glm::mat4(1.0f);
}

Engine::Object::~Object()
{
    //if (position != NULL) delete position;
    //if (rotation != NULL) delete rotation;
    //if (scale != NULL) delete scale;
    //if (children != NULL) delete children; // jesus christ !
    //if (model != NULL) delete model;
}

Engine::Object::Object(const Object& other)
{
    Object object;
    object = other;
    *this->position = *object.GetPosition();
    *this->rotation = *object.GetRotation();
    *this->scale = *object.GetScale();
}

Engine::Object::Object(float* position)
{
    this->position = position;
    this->rotation = new float[3] {0.0f, 0.0f, 0.0f};
    this->scale = new float[3] {1.0f, 1.0f, 1.0f};

    this->isMoving = false;
    this->model = nullptr;
    this->children = nullptr;

    this->UpdateTransform();
}

Engine::Object::Object(float* position, float* rotation, float* scale)
{
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;

    this->isMoving = false;
    this->model = nullptr;
    this->children = nullptr;

    this->UpdateTransform();
}

Engine::Object& Engine::Object::operator=(const Object& rhs)
{
    if (this == &rhs) return *this;
    return *this;
}

float* Engine::Object::GetPosition()
{
    return position;
}

float* Engine::Object::GetRotation()
{
    return rotation;
}

float* Engine::Object::GetScale()
{
    return scale;
}

void Engine::Object::SetPosition(float* position)
{
    this->position = position;
    this->UpdateTransform();
}

void Engine::Object::SetRotation(float* rotation)
{
    this->rotation = rotation;
    this->UpdateTransform();
}

void Engine::Object::SetScale(float* scale)
{
    this->scale = scale;
    this->UpdateTransform();
}

void Engine::Object::MoveRelative(float* offset)
{
    position[0] += offset[0];
    position[1] += offset[1];
    position[2] += offset[2];

    this->UpdateTransform();
}

void Engine::Object::MoveAbsolute(float* location)
{
    this->position = location;
    this->UpdateTransform();
}

void Engine::Object::RotateRelative(float* offset)
{
    rotation[0] += offset[0];
    rotation[1] += offset[1];
    rotation[2] += offset[2];

    this->UpdateTransform();
}

void Engine::Object::RotateAbsolute(float* rotation)
{
    this->rotation = rotation;
    this->UpdateTransform();
}

void Engine::Object::ScaleRelative(float* offset)
{
    scale[0] += offset[0];
    scale[1] += offset[1];
    scale[2] += offset[2];

    this->UpdateTransform();
}

void Engine::Object::ScaleAbsolute(float* scale)
{
    this->scale = scale;
    this->UpdateTransform();
}

void Engine::Object::Transform(glm::mat4 transformMatrix)
{
    glm::vec3 translation;
    glm::quat orientation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::vec3 rotation;

    glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);
    rotation = glm::eulerAngles(orientation);
    rotation = glm::degrees(rotation);

    this->position = glm::value_ptr(translation);
    this->rotation = glm::value_ptr(rotation);
    this->scale = glm::value_ptr(scale);

    this->UpdateTransform();
}

glm::mat4* Engine::Object::GetTransform()
{
    return &this->transform;
}

void Engine::Object::UpdateTransform()
{
    glm::mat4 trans = glm::mat4(1.0f);

    trans = glm::translate(trans, glm::vec3(position[0], position[1], position[2]));
    trans = glm::scale(trans, glm::vec3(scale[0], scale[1], scale[2]));
    trans = glm::rotate(trans, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::rotate(trans, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));

    this->transform = trans;
}

void Engine::Object::SetShader(const Engine::Shader& shader)
{
    this->shader = shader;
}

Engine::Shader* Engine::Object::GetShader()
{
    return &this->shader;
}

void Engine::Object::SetModel(const Engine::Model& model)
{
    this->model = model;
}

Engine::Model* Engine::Object::GetModel()
{
    return &this->model;
}

void Engine::Object::Draw(glm::mat4 view, glm::mat4 projection)
{
    //find the locations of uniform variables in the shader and assign transform matrices to them

    int modelLoc = glGetUniformLocation(this->shader.GetProgramID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->transform));

    int viewLoc = glGetUniformLocation(this->shader.GetProgramID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(this->shader.GetProgramID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    model.Draw(&this->shader);
}

std::string Engine::Object::ToString()
{
    std::string str = "Class name: ";
    str.append(typeid(this).name());
    str += ", With size: ";
    str.append(std::to_string(sizeof(this)));

    return str;
}
