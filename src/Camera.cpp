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
    this->aspectRatio = aspectRatio;
    this->nearClip = nearClip;
    this->farclip = farClip;
    this->fov = fov;

    this->UpdateProjection();
    
    this->upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Engine::Camera::GetForwardDirection()
{
    //camera direction is in essence the normalized rotation component
    glm::vec3 direction = glm::normalize(this->rotation);
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

glm::mat4 Engine::Camera::GetView()
{
    //direction is just our normalized orientation
    glm::vec3 direction = glm::normalize(this->rotation);
    //building the view matrix using position, direction and Up direction
    glm::mat4 view = glm::lookAt(this->position, this->position + direction, this->upDirection);
    return view;
}

glm::vec4* Engine::Camera::GetFrustumPlanes()
{
    //full camera matrix from which we'll extract the planes
    //OpenGL uses collumn-major matrice orientation, so we need to transpose it before we can extract
    glm::mat4 matrix = this->projection * this->GetView();
    matrix = glm::transpose(matrix);

    glm::vec4 left{matrix[3] + matrix[0]};
    glm::vec4 right{matrix[3] - matrix[0]};

    glm::vec4 top{matrix[3] - matrix[1]};
    glm::vec4 bottom{matrix[3] + matrix[1]};

    glm::vec4 near{matrix[3] + matrix[2]};
    glm::vec4 far{matrix[3] - matrix[2]};

    //array of planes to fill in
    glm::vec4 planes[6] = {glm::normalize(left), glm::normalize(right), glm::normalize(top),
                           glm::normalize(bottom), glm::normalize(near), glm::normalize(far)};
    
    return planes;
}

void Engine::Camera::Draw(Engine::BoundingBox* bb)
{
    //calculate the size of this bounding box, will be useful for vertex calculations and transform matrix
    glm::vec3 sizeOffset = glm::vec3(bb->maxs - bb->mins);
    float xOffset = sizeOffset.x;
    float yOffset = sizeOffset.y;
    float zOffset = sizeOffset.z;

    //build a transform matrix for the bounding box
    glm::mat4 transform(1.0f);

    //get whatever shader is currently being used, it should do for wireframe rendering
    Engine::Shader* currentShader = Engine::Shader::GetCurrentShader();

    //pass the model, view and projection (MVP) matrices to the shader
    glUniformMatrix4fv(currentShader->GetAttributeLocation(Engine::Shader::ShaderAttribute::MODEL_LOCATION), 1, GL_FALSE, glm::value_ptr(transform));

    glUniformMatrix4fv(currentShader->GetAttributeLocation(Engine::Shader::ShaderAttribute::VIEW_LOCATION), 1, GL_FALSE, glm::value_ptr(this->GetView()));

    glUniformMatrix4fv(currentShader->GetAttributeLocation(Engine::Shader::ShaderAttribute::PROJECTION_LOCATION), 1, GL_FALSE, glm::value_ptr(this->GetProjection()));

    //create vertex buffer and element buffer objects
    unsigned int VBO;
    unsigned int EBO;

    //form a full array of vertices from the two bounding ones
    //starting at mins, clockwise order +Y
    float vertices[] = {bb->mins.x, bb->mins.y, bb->mins.z,
                        bb->mins.x, bb->mins.y, bb->mins.z + zOffset,
                        bb->mins.x + xOffset, bb->mins.y, bb->mins.z + zOffset,
                        bb->mins.x + xOffset, bb->mins.y, bb->mins.z,
                        bb->mins.x, bb->mins.y + yOffset, bb->mins.z,
                        bb->mins.x, bb->mins.y + yOffset, bb->mins.z + zOffset,
                        bb->maxs.x, bb->maxs.y, bb->maxs.z,
                        bb->mins.x + xOffset, bb->mins.y + yOffset, bb->mins.z};

    //form quads out of vertices
    unsigned int quads[] = { 0, 4, 7, 3,
                            3, 7, 6, 2,
                            2, 6, 5, 1,
                            1, 5, 4, 0 };
    //generate the buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //fill the vertex buffer with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //setup vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //bind the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //fill the element buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quads), quads, GL_DYNAMIC_DRAW);

    //set the rendering mode to wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //use whatever current shader we managed to retrieve
    glUseProgram(currentShader->GetProgramID());

    //render
    glDrawElements(GL_QUADS, sizeof(quads) / sizeof(quads[0]), GL_UNSIGNED_INT, 0);

    //revert back to normal rendering mode so we dont screw up everyone else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //clean up
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

glm::mat4 Engine::Camera::GetProjection()
{
    return this->projection;
}

float Engine::Camera::GetSpeed()
{
    return this->speed;
}

float Engine::Camera::GetAspectRatio()
{
    return this->aspectRatio;
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

void Engine::Camera::UpdateProjection()
{
    this->projection = glm::perspective(this->fov, this->aspectRatio, this->nearClip, this->farclip);
}

void Engine::Camera::SetSpeed(float speed)
{
    this->speed = speed;
}

void Engine::Camera::SetAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    this->UpdateProjection();
}

void Engine::Camera::SetNearClip(float nearClip)
{
    this->nearClip = nearClip;
    this->UpdateProjection();
}

void Engine::Camera::SetFarClip(float farClip)
{
    this->farclip = farClip;
    this->UpdateProjection();
}

void Engine::Camera::SetFov(float fov)
{
    this->fov = fov;
    this->UpdateProjection();
}

void Engine::Camera::Draw(Engine::Actor* actor)
{
    //find the locations of uniform variables in the shader and assign transform matrices to them
    int modelLoc = glGetUniformLocation(actor->GetShader()->GetProgramID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(actor->GetTransform()));

    //check if the uniform variable was found
    if (modelLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of model uniform var was not found" << std::endl;
        return;
    }

    int viewLoc = glGetUniformLocation(actor->GetShader()->GetProgramID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(this->GetView()));

    //check if the uniform variable was found
    if (viewLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of view uniform var was not found" << std::endl;
        return;
    }

    int projectionLoc = glGetUniformLocation(actor->GetShader()->GetProgramID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(this->GetProjection()));

    //check if the of uniform variable was found
    if (projectionLoc < 0) {
        std::cerr << "Unable to draw object: " << this->ToString() << ", location of projection uniform var was not found" << std::endl;
        return;
    }

    //pass the draw call to the actor's encapsulated model
    actor->GetModel()->Draw(actor->GetShader());
}

void Engine::Camera::Draw(std::vector<Engine::Actor*> actors)
{
    for (size_t i = 0; i < actors.size(); i++)   
        this->Draw(actors[i]);
}