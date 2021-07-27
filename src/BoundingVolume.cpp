#include "BoundingVolume.h"

void Engine::BoundingBox::Move(glm::vec3 offset)
{
    this->mins += offset;
    this->maxs += offset;
}

void Engine::BoundingBox::Move(glm::vec3 direction, float magnitude)
{
    this->mins += glm::normalize(direction) * magnitude;
    this->maxs += glm::normalize(direction) * magnitude;
}

void Engine::BoundingSphere::Move(glm::vec3 offset)
{
    this->center += offset;
    this->center += offset;
}

void Engine::BoundingSphere::Move(glm::vec3 direction, float magnitude)
{
    this->center += glm::normalize(direction) * magnitude;
    this->center += glm::normalize(direction) * magnitude;
}

bool Engine::BoundingBox::IntersectsWith(Engine::BoundingBox& other)
{
    //for an intersection to occur, it needs to happen on all three axes
    //for performance reasons, we split the check into three parts, if any of them return false, the whole function can imediatelly return false
    //we assume that multiple boxes can have perfectly aligned walls, which we count as an intersection, hence the (<=, >=) instead of (<, >)

    //x axis
    bool intersection = this->GetLeft() <= other.GetRight() && this->GetRight() >= other.GetLeft();

    if (!intersection)
        return false;
    else {
        //y axis
        intersection = this->GetBottom() <= other.GetTop() && this->GetTop() >= other.GetBottom();

        if (!intersection)
            return false;
        else {
            //z axis
            intersection = this->GetFront() <= other.GetBack() && this->GetBack() >= other.GetFront();

            if (!intersection)
                return false;
            else
                return true;
        }
    }
}

bool Engine::BoundingBox::IntersectsWith(Engine::BoundingSphere& sphere)
{
    float distance = glm::pow(sphere.radius, 2);

    //x axis
    if (sphere.center.x < this->mins.x)
        distance -= glm::pow(sphere.center.x - this->mins.x, 2);
    else
        if (sphere.center.x > this->maxs.x)
            distance -= glm::pow(sphere.center.x - this->maxs.x, 2);

    //y axis
    if (sphere.center.y < this->mins.y)
        distance -= glm::pow(sphere.center.y - this->mins.y, 2);
    else
        if (sphere.center.y > this->maxs.y)
            distance -= glm::pow(sphere.center.y - this->maxs.y, 2);

    //z axis
    if (sphere.center.z < this->mins.z)
        distance -= glm::pow(sphere.center.z - this->mins.z, 2);
    else
        if (sphere.center.z > this->maxs.z)
            distance -= glm::pow(sphere.center.z - this->maxs.z, 2);

    return distance > 0;
}

bool Engine::BoundingSphere::IntersectsWith(Engine::BoundingSphere& other)
{
    //check if the sum of radi is greater than the distance between the spheres
    return glm::distance(this->center, other.center) < (this->radius + other.radius);
}

bool Engine::BoundingSphere::IntersectsWith(Engine::BoundingBox& bBox)
{
    //just reuse the intersection algorithm from the bounding boxes perspective
    return bBox.IntersectsWith(*this);
}

void Engine::BoundingBox::Draw(Engine::Camera* camera)
{
    //calculate the size of this bounding box, will be useful for vertex calculations and transform matrix
    glm::vec3 sizeOffset = glm::vec3(maxs - mins);
    float xOffset = sizeOffset.x;
    float yOffset = sizeOffset.y;
    float zOffset = sizeOffset.z;

    //build a transform matrix for the bounding box
    glm::mat4 transform(1.0f);

    //get whatever shader is currently being used, it should do for wireframe rendering
    int currentShader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentShader);

    //pass the model, view and projection (MVP) matrices to the shader
    int modelLoc = glGetUniformLocation(currentShader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

    int viewLoc = glGetUniformLocation(currentShader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetView()));

    int projectionLoc = glGetUniformLocation(currentShader, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjection()));

    //create vertex buffer and element buffer objects
    unsigned int VBO;
    unsigned int EBO;

    //form a full array of vertices from the two bounding ones
    //starting at mins, clockwise order +Y
    float vertices[] = {mins.x, mins.y, mins.z,
                        mins.x, mins.y, mins.z + zOffset,
                        mins.x + xOffset, mins.y, mins.z + zOffset,
                        mins.x + xOffset, mins.y, mins.z,
                        mins.x, mins.y + yOffset, mins.z,
                        mins.x, mins.y + yOffset, mins.z + zOffset,
                        maxs.x, maxs.y, maxs.z,
                        mins.x + xOffset, mins.y + yOffset, mins.z};

    //form quads out of vertices
    unsigned int quads[] = {0, 4, 7, 3,
                            3, 7, 6, 2,
                            2, 6, 5, 1,
                            1, 5, 4, 0};
    //generate the buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //fill the vertex buffer with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //setup vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //bind the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //fill the element buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quads), quads, GL_STATIC_DRAW);

    //set the rendering mode to wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //use whatever current shader we managed to retrieve
    glUseProgram(currentShader);

    //render
    glDrawElements(GL_QUADS, sizeof(quads) / sizeof(quads[0]), GL_UNSIGNED_INT, 0);

    //revert back to normal rendering mode so we dont screw up everyone else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //clean up
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

float Engine::BoundingBox::GetBottom() {

    return this->mins.y;
}

float Engine::BoundingBox::GetLeft() {

    return this->mins.x;
}

float Engine::BoundingBox::GetFront() {

    return this->mins.z;
}

float Engine::BoundingBox::GetTop() {

    return this->maxs.y;
}

float Engine::BoundingBox::GetRight() {

    return this->maxs.x;
}

float Engine::BoundingBox::GetBack() {

    return this->maxs.z;
}