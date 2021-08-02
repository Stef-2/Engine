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
    //we can extract our forward looking direction from the view matrix
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

glm::mat4 Engine::Camera::GetView()
{
    //we're way too cool to construct a view matrix using LookAt(), so, 
    //convert our orientation quaternion into a rotation matix
    glm::mat4 direction = glm::mat4_cast(this->orientation);
    
    //construct a translation matrix from our position
    glm::mat4 position = glm::mat4(1.0f);
    position = glm::translate(position, -this->position);

    //multiply the two to create a view matrix B-)
    return direction * position;
}

glm::vec4* Engine::Camera::GetFrustumPlanes()
{
    //full camera matrix from which we'll extract the planes
    glm::mat4 matrix = this->projection * this->GetView();
    //OpenGL uses collumn-major matrix orientation, so we need to transpose it before extraction
    matrix = glm::transpose(matrix);

    glm::vec4 left {matrix[3] + matrix[0]};
    glm::vec4 right {matrix[3] - matrix[0]};

    glm::vec4 top {matrix[3] - matrix[1]};
    glm::vec4 bottom {matrix[3] + matrix[1]};

    glm::vec4 near {matrix[3] + matrix[2]};
    glm::vec4 far {matrix[3] - matrix[2]};

    //array of planes to fill in
    //normalizing them means that the first three values of each plane will be their respective normal
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

    //use whatever current shader we managed to retrieve
    glUseProgram(currentShader->GetProgramID());

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

    //render
    glDrawElements(GL_QUADS, sizeof(quads) / sizeof(quads[0]), GL_UNSIGNED_INT, 0);

    //revert back to normal rendering mode so we dont screw up everyone else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //clean up
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Engine::Camera::Draw(Engine::Skybox* skybox)
{
    //we trick the shader into thinking the skybox has infinite / max depth by removing the last row and collumn from the view matrix
    //we can do this by converting the 4x4 matrix into a 3x3 one and then back
    glm::mat4 view = glm::mat4(glm::mat3(this->GetView()));
    glDepthMask(GL_FALSE);
    skybox->GetShader()->Activate();

    glUniformMatrix4fv(skybox->GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::VIEW_LOCATION), 1, GL_FALSE, glm::value_ptr(view));

    glUniformMatrix4fv(skybox->GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::PROJECTION_LOCATION), 1, GL_FALSE, glm::value_ptr(this->GetProjection()));

    //bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, skybox->GetVBO());
    //fill the vertex buffer with data
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), skybox->GetVertices(), GL_DYNAMIC_DRAW);
    //setup vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->GetTexture()->GetTextureID());

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
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
    actor->GetShader()->Activate();

    //find the locations of uniform variables in the shader and assign transform matrices to them

    glUniformMatrix4fv(actor->GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::MODEL_LOCATION), 1, GL_FALSE, glm::value_ptr(actor->GetTransform()));

    glUniformMatrix4fv(actor->GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::VIEW_LOCATION), 1, GL_FALSE, glm::value_ptr(this->GetView()));

    glUniformMatrix4fv(actor->GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::PROJECTION_LOCATION), 1, GL_FALSE, glm::value_ptr(this->GetProjection()));

    //pass the draw call to the encapsulated model
    actor->GetModel()->Draw(actor->GetShader());
}

void Engine::Camera::Draw(std::vector<Engine::Actor*> actors)
{
    for (size_t i = 0; i < actors.size(); i++)   
        this->Draw(actors[i]);
}