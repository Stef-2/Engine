#include "Renderer.h"

Engine::Renderer::Renderer()
{
    this->colorDepth = 24;
}

std::vector<Engine::Actor*> Engine::Renderer::FrustumCull(Engine::Camera& camera, std::vector<Engine::Actor*> actors)
{
    //get the frustum clipping planes from the camera
    glm::vec4* frustumPlanes = camera.GetFrustumPlanes();

    glm::vec3 front;
    
    bool visible = true;

    //number of accumulated frustum culls that we can use to show off
    unsigned int numCulls = 0;

    //list of visible actors
    std::vector<Engine::Actor*> visibleActors;

    //go through all the actors
    for (size_t i = 0; i < actors.size(); i++)
    {
        //go through all 6 camera frustum planes
        for (size_t j = 0; j < 6; j++)
        {
            //assume the object is visible until proven otherwise
            visible = true;

            //find the vertex of object's bounding box that is farthest along the current frustum plane's normal
            front = actors.at(i)->GetModel()->GetBoundingBox()->mins;

            if (frustumPlanes[j].x >= 0)
                front.x = actors.at(i)->GetModel()->GetBoundingBox()->maxs.x;
            if (frustumPlanes[j].y >= 0)
                front.y = actors.at(i)->GetModel()->GetBoundingBox()->maxs.y;
            if (frustumPlanes[j].z >= 0)
                front.z = actors.at(i)->GetModel()->GetBoundingBox()->maxs.z;
            
            //if the distance from the furthest point to the current plane is less than zero, the whole object is outside the frustum
            if (glm::dot(glm::vec3(frustumPlanes[j]),front) + frustumPlanes[j].w < 0.0f) {
                visible = false;
                numCulls += 1;

                //we don't actually need to check every plane, one is enough
                //so long as we can prove the object is fully behind any one of them, we can skip the others
                break;
            }
        }

        //otherwise, the object is visible and should be rendered
        if (visible) {
            visibleActors.push_back(actors.at(i));
        }
    }

    return visibleActors;
}

void Engine::Renderer::Render(Engine::Camera& camera, Engine::Actor& actor)
{
    actor.GetShader()->Activate();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //find the locations of uniform variables in the shader and assign transform matrices to them
    glUniformMatrix4fv(actor.GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::MODEL_LOCATION), 1, GL_FALSE, glm::value_ptr(actor.GetTransform()));

    glUniformMatrix4fv(actor.GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::VIEW_LOCATION), 1, GL_FALSE, glm::value_ptr(camera.GetView()));

    glUniformMatrix4fv(actor.GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::PROJECTION_LOCATION), 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));

    //find and bind the appropriate shader attribute positions
    glBindAttribLocation(actor.GetShader()->GetProgramID(), actor.GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::VERTEX_POSITION_LOCATION), "vertPos");

    glBindAttribLocation(actor.GetShader()->GetProgramID(), actor.GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::VERTEX_NORMAL_LOCATION), "vertNormal");

    glBindAttribLocation(actor.GetShader()->GetProgramID(), actor.GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::VERTEX_UV_LOCATION), "vertCoord");

    //go through all the meshes in actor's model and draw them
    for (size_t i = 0; i < actor.GetModel()->GetMeshes()->size(); i++) {

        //bind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, actor.GetModel()->GetMeshes()->at(i).GetVBO());

        //setup vertex position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)0);
        //glEnableVertexAttribArray(0);

        //setup vertex normal attribute
        //struct members are sequential in memory so we can use offsetof() to find the appropriate data
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, normal));
        //glEnableVertexAttribArray(1);

        //setup vertex UVs attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, uv));
        //glEnableVertexAttribArray(2);

        //bind the element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, actor.GetModel()->GetMeshes()->at(i).GetEBO());

        //bind the corresponding texture
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, material->GetDiffuse()->GetTextureID());
        glBindTexture(GL_TEXTURE_2D, actor.GetModel()->GetMaterials()->at(0).GetDiffuse()->GetTextureID());

        //render
        glDrawElements(GL_TRIANGLES, actor.GetModel()->GetMeshes()->at(i).GetIndices()->size(), GL_UNSIGNED_INT, 0);

        //clean up
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
}

void Engine::Renderer::Render(Engine::Camera& camera, Engine::BoundingBox& box)
{
    //calculate the size of this bounding box, will be useful for vertex calculations and transform matrix
    glm::vec3 sizeOffset = glm::vec3(box.maxs - box.mins);
    float xOffset = sizeOffset.x;
    float yOffset = sizeOffset.y;
    float zOffset = sizeOffset.z;

    //build a transform matrix for the bounding box
    glm::mat4 transform(1.0f);

    //get whatever shader is currently being used, it should do for simple wireframe rendering
    Engine::Shader* currentShader = Engine::Shader::GetCurrentShader();

    //use whatever current shader we managed to retrieve
    glUseProgram(currentShader->GetProgramID());

    //pass the model, view and projection (MVP) matrices to the shader
    glUniformMatrix4fv(currentShader->GetAttributeLocation(Engine::Shader::ShaderAttribute::MODEL_LOCATION), 1, GL_FALSE, glm::value_ptr(transform));

    glUniformMatrix4fv(currentShader->GetAttributeLocation(Engine::Shader::ShaderAttribute::VIEW_LOCATION), 1, GL_FALSE, glm::value_ptr(camera.GetView()));

    glUniformMatrix4fv(currentShader->GetAttributeLocation(Engine::Shader::ShaderAttribute::PROJECTION_LOCATION), 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));

    //create vertex buffer and element buffer objects
    unsigned int VBO;
    unsigned int EBO;

    //form a full array of vertices from the two bounding ones
    //starting at mins, clockwise order +Y
    float vertices[] = { box.mins.x, box.mins.y, box.mins.z,
                        box.mins.x, box.mins.y, box.mins.z + zOffset,
                        box.mins.x + xOffset, box.mins.y, box.mins.z + zOffset,
                        box.mins.x + xOffset, box.mins.y, box.mins.z,
                        box.mins.x, box.mins.y + yOffset, box.mins.z,
                        box.mins.x, box.mins.y + yOffset, box.mins.z + zOffset,
                        box.maxs.x, box.maxs.y, box.maxs.z,
                        box.mins.x + xOffset, box.mins.y + yOffset, box.mins.z };

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

    //render
    glDrawElements(GL_QUADS, sizeof(quads) / sizeof(quads[0]), GL_UNSIGNED_INT, 0);

    //revert back to normal rendering mode so we dont screw up everyone else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //clean up
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Engine::Renderer::Render(Engine::Camera& camera, Engine::Skybox& skybox)
{
    //we can trick the shader into thinking the skybox has infinite / max depth by removing the last row and collumn from the view matrix
    //we can do that by converting the 4x4 matrix into a 3x3 one and then back
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetView()));
    glDepthMask(GL_FALSE);
    skybox.GetShader()->Activate();

    glUniformMatrix4fv(skybox.GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::VIEW_LOCATION), 1, GL_FALSE, glm::value_ptr(view));

    glUniformMatrix4fv(skybox.GetShader()->GetAttributeLocation(Engine::Shader::ShaderAttribute::PROJECTION_LOCATION), 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));

    //bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, skybox.GetVBO());
    //fill the vertex buffer with data
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), skybox.GetVertices(), GL_DYNAMIC_DRAW);
    //setup vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTexture()->GetTextureID());

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
    glDisableVertexAttribArray(0);
}


void Engine::Renderer::Render(Engine::Camera& camera, std::vector<Engine::Actor*> actors)
{
    for (size_t i = 0; i < actors.size(); i++)
        this->Render(camera, *actors[i]);
}

void Engine::Renderer::SetColorDepth(int colorDepth)
{
    this->colorDepth = colorDepth;
}
int Engine::Renderer::GetColorDepth()
{
    return this->colorDepth;
}
