#include "Renderer.h"

Engine::Renderer::Renderer() 
	/*wireFrameShader{ "C:\\Users\\Stefan\\source\\repos\\Engine\\shaders\\wireframe.vert" ,
					 "C:\\Users\\Stefan\\source\\repos\\Engine\\shaders\\wireframe.frag" },*/
	/*shadowMapShader{ "C:\\Users\\Stefan\\source\\repos\\Engine\\shaders\\shadowmap.vert",
					 "C:\\Users\\Stefan\\source\\repos\\Engine\\shaders\\shadowmap.frag" }*/
{
	this->colorDepth = 24u;
	
	// get OpenGL implementation limits for rendering related info
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &this->maxFragmentTextureUnits);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &this->maxTotalTextureUnits);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->max1DTextureSize);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->max2DTextureSize);
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &this->max3DTextureSize);
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &this->maxCubMapTextureSize);
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &this->maxArrayTextureLayers);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &this->maxFrameBufferWidth);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &this->maxFrameBufferHeight);
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &this->maxRenderBufferSize);
	glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &this->maxTextureBuferSize);
	glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &this->maxFragmentInputs);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &this->maxVertexAttributes);
	glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &this->maxVertexOutputs);
}

std::vector<Engine::Actor*> Engine::Renderer::FrustumCull(const Engine::Camera& camera, const std::vector<Engine::Actor*> actors)
{
	// get the frustum clipping planes from the camera
	std::vector<glm::vec4> frustumPlanes = camera.GetFrustumPlanes();

	glm::vec3 front;
	
	bool visible = true;

	// number of accumulated frustum culls that we can use to show off
	unsigned int numCulls = 0;

	// list of visible actors
	std::vector<Engine::Actor*> visibleActors;

	// go through all the actors
	for (size_t i = 0; i < actors.size(); i++)
	{
		// go through all 6 camera frustum planes
		for (size_t j = 0; j < 6; j++)
		{
			// assume the object is visible until proven otherwise
			visible = true;

			// find the vertex of object's bounding box that is farthest along the current frustum plane's normal
			front = actors.at(i)->GetModel().GetBoundingBox().minimums;

			if (frustumPlanes[j].x >= 0)
				front.x = actors.at(i)->GetModel().GetBoundingBox().maximums.x;
			if (frustumPlanes[j].y >= 0)
				front.y = actors.at(i)->GetModel().GetBoundingBox().maximums.y;
			if (frustumPlanes[j].z >= 0)
				front.z = actors.at(i)->GetModel().GetBoundingBox().maximums.z;
			
			// if the distance from the furthest point to the current plane is less than zero, the whole object is outside the frustum
			if (glm::dot(glm::vec3(frustumPlanes[j]),front) + frustumPlanes[j].w < 0.0f) {
				visible = false;
				numCulls += 1;

				// we don't actually need to check every plane, one is enough
				// so long as we can prove the object is fully behind any one of them, we can skip the others
				break;
			}
		}

		// otherwise, the object is visible and should be rendered
		if (visible) {
			visibleActors.push_back(actors.at(i));
		}
	}

	return visibleActors;
}

void Engine::Renderer::Render(const Engine::Camera& camera, Engine::Actor& actor)
{
	actor.GetShader().Activate();

	// find the locations of uniform variables in the shader and assign transform matrices to them
	glBindBuffer(GL_UNIFORM_BUFFER, Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::MVP_MATRICES));

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(actor.GetTransform()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetView()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(camera.GetProjection()));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	// hack in a cubemap, do this properly later
	if (actor.GetShader().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::CUBE_MAP))
	{
		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Engine::Skybox::GetActiveSkybox().GetTexture().GetTextureID());
		glUniform1i(actor.GetShader().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::CUBE_MAP), 6);
	}

	actor.GetShader().SetShaderFlag(Engine::ShaderProgram::ShaderFlag::STATIC);

	// go through all static meshes in actor's model and draw them
	for (size_t i = 0; i < actor.GetModel().GetStaticMeshes().size(); i++) {

		// bind the vertex array buffer
		glBindVertexArray(actor.GetModel().GetStaticMeshes().at(i)->GetVertexArrayBuffer());

		// bind the corresponding materials if they exist
		if (&actor.GetModel().GetStaticMeshes().at(i)->GetMaterial())
			actor.GetModel().GetStaticMeshes().at(i)->GetMaterial().Activate(actor.GetShader());

		// render
		if (actor.GetModel().GetStaticMeshes().at(i)->GetInstanceable()) {
			auto type = actor.GetModel().GetStaticMeshes().at(i)->GetInstanceType();
			glDrawElementsInstanced(GL_TRIANGLES, actor.GetModel().GetStaticMeshes().at(i)->GetIndices().size(), GL_UNSIGNED_INT, 0, actor.GetModel().GetStaticMeshes().at(i)->GetInstances<Engine::SimpleInstance>().size());
		}
		//glDrawElements(GL_TRIANGLES, actor.GetModel().GetStaticMeshes().at(i)->GetIndices().size(), GL_UNSIGNED_INT, 0);
	}

	actor.GetShader().SetShaderFlag(Engine::ShaderProgram::ShaderFlag::ANIMATED);

	// go through all animated meshes in actor's model and draw them
	for (size_t i = 0; i < actor.GetModel().GetAnimatedMeshes().size(); i++) {

		Engine::Skeleton& skeleton = actor.GetModel().GetAnimatedMeshes().at(i)->GetSkeleton();

		// make a stack of bone transforms that we'll send to the GPU
		glm::mat4* bones = new glm::mat4[skeleton.GetBones().size()];

		#pragma omp simd
		for (unsigned int j = 0; j < skeleton.GetBones().size(); j++)
			bones[j] = skeleton.GetFinalBoneTransformAnimated(j);

		glUniformMatrix4fv(actor.GetShader().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::BONE_TRANSFORMATIONS), skeleton.GetBones().size(), GL_FALSE,
			glm::value_ptr(bones[0]));

		delete[]bones;

		// bind the vertex array buffer
		glBindVertexArray(actor.GetModel().GetAnimatedMeshes().at(i)->GetVertexArrayBuffer());

		// bind the corresponding texture(s)
		actor.GetModel().GetAnimatedMeshes().at(i)->GetMaterial().Activate(actor.GetShader());

		// render
		glDrawElements(GL_TRIANGLES, actor.GetModel().GetAnimatedMeshes().at(i)->GetIndices().size(), GL_UNSIGNED_INT, 0);
	}
}

void Engine::Renderer::Render(Engine::UserInterface& UI)
{
	auto* panel = dynamic_cast<Engine::UserInterface::Panel*>(Engine::UserInterface::GetVisibleElements().at(0));

	if (panel)
	{
		panel->GetShader()->Activate();
	}

	glBindVertexArray(UI.GetSharedQuad().GetVertexArrayBuffer());
	//std::cout << "elements: " << UI.GetVisibleElements().size() << std::endl;
	glDrawElementsInstanced(GL_TRIANGLES, UI.GetSharedQuad().GetIndices().size(), GL_UNSIGNED_INT, 0, Engine::UserInterface::GetVisibleElements().size());

	/*
	for (auto& element : Engine::UserInterface::GetVisibleElements())
	{
		// see what we're type of UIElement it is
		auto* panel = dynamic_cast<Engine::UserInterface::Panel*>(element);

		if (panel)
		{
			panel->GetShader()->
		}
	}*/
}

void Engine::Renderer::Render(const Engine::Camera& camera, const Engine::BoundingBox& box)
{
	// calculate the size of this bounding box, will be useful for vertex calculations and transform matrix
	glm::vec3 sizeOffset = glm::vec3(box.maximums - box.minimums);
	float xOffset = sizeOffset.x;
	float yOffset = sizeOffset.y;
	float zOffset = sizeOffset.z;

	// build an identity transform matrix for the bounding box
	glm::mat4 transform(1.0f);

	// use the wireframe utility shader
	this->wireFrameShader.Activate();

	// pass the model, view and projection (MVP) matrices to the shader
	glUniformMatrix4fv(this->wireFrameShader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::MODEL_LOCATION), 1, GL_FALSE, glm::value_ptr(transform));

	glUniformMatrix4fv(this->wireFrameShader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::VIEW_LOCATION), 1, GL_FALSE, glm::value_ptr(camera.GetView()));

	glUniformMatrix4fv(this->wireFrameShader.GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::PROJECTION_LOCATION), 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));

	// create vertex buffer and element buffer objects
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	// form a full array of vertices from the two bounding ones
	// starting at mins, clockwise order +Y
	float vertices[] = { box.minimums.x, box.minimums.y, box.minimums.z,
						 box.minimums.x, box.minimums.y, box.minimums.z + zOffset,
						 box.minimums.x + xOffset, box.minimums.y, box.minimums.z + zOffset,
						 box.minimums.x + xOffset, box.minimums.y, box.minimums.z,
						 box.minimums.x, box.minimums.y + yOffset, box.minimums.z,
						 box.minimums.x, box.minimums.y + yOffset, box.minimums.z + zOffset,
						 box.maximums.x, box.maximums.y, box.maximums.z,
						 box.minimums.x + xOffset, box.minimums.y + yOffset, box.minimums.z };

	// form a line strip out of vertices to simulate quad drawing
	// there is a very slight overhead here as 3 lines have to be drawn twice
	unsigned int quads[] = { 0, 1, 2, 3,
							 0, 4, 5, 6,
							 7, 4, 5, 1,
							 2, 6, 7, 3 };
	// generate the buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the vertex array
	glBindVertexArray(VAO);

	// bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// setup vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// fill the vertex buffer with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	
	// bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// fill the element buffer with data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quads), quads, GL_DYNAMIC_DRAW);

	// set the rendering mode to wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render
	glDrawElements(GL_LINE_STRIP, sizeof(quads) / sizeof(*quads), GL_UNSIGNED_INT, 0);

	// revert back to normal rendering mode so we dont screw up everyone else
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// clean up
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Engine::Renderer::Render(const Engine::Camera& camera, Engine::Skybox& skybox)
{
	glDepthFunc(GL_LEQUAL);
	skybox.GetShader().Activate();

	// we trick the shader into thinking the skybox is moving along with the camera by removing its position data from the view transform matrix
	// we can do that by converting the 4x4 view matrix into a 3x3 one and then back
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetView()));

	glUniformMatrix4fv(skybox.GetShader().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::VIEW_LOCATION), 1, GL_FALSE, glm::value_ptr(view));

	glUniformMatrix4fv(skybox.GetShader().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::PROJECTION_LOCATION), 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));

	// bind the vertex array buffer
	glBindVertexArray(skybox.GetVAO());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTexture().GetTextureID());
	glUniform1i(skybox.GetShader().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::CUBE_MAP), 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS);
}


void Engine::Renderer::Render(const Engine::Camera& camera, const std::vector<Engine::Actor*>& actors)
{
	for (size_t i = 0; i < actors.size(); i++) {
		this->Render(camera, *actors[i]);
	}
}

void Engine::Renderer::Render(const Engine::Camera& camera, Engine::Skeleton& skeleton)
{
	for (size_t i = 0; i < skeleton.GetBones().size(); i++)
	{
		glm::mat4 matrix = skeleton.GetBones()[i].GetGlobalTransformAnimated();
		//glm::vec3 bonePosition = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
		glm::vec3 bonePosition = glm::vec3(skeleton.GetFinalBoneTransformAnimated(i)[3][0], skeleton.GetFinalBoneTransformAnimated(i)[3][1], skeleton.GetFinalBoneTransformAnimated(i)[3][2]);
		//std::cout << skeleton.GetBones().at(i).GetNode().GetName() << " - Position: " << glm::to_string(bonePosition) << std::endl;
		Engine::BoundingBox box(bonePosition - 0.5f, bonePosition + 0.5f);

		//if (bonePosition.y < 0) std::cout << "BONE : " << skeleton.GetBones().at(i).GetNode().GetName() << " IS IN BANGLADESH: " << bonePosition.y << std::endl;
		this->Render(camera, box);
	}
}

void Engine::Renderer::Render(const Engine::Camera& camera, Engine::PointLight& light)
{
	Engine::BoundingBox box{light.GetPosition() - 0.5f, light.GetPosition() + 0.5f};
	this->Render(camera, box);
}

void Engine::Renderer::Render(const Engine::Camera& camera, Engine::Terrain& terrain)
{
	terrain.GetShader().Activate();

	// find the locations of uniform variables in the shader and assign transform matrices to them
	glBindBuffer(GL_UNIFORM_BUFFER, Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::MVP_MATRICES));

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(terrain.GetTransform()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetView()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(camera.GetProjection()));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Engine::Skybox::GetActiveSkybox().GetTexture().GetTextureID());
	glUniform1i(terrain.GetShader().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::CUBE_MAP), 6);

	terrain.GetShader().SetShaderFlag(Engine::ShaderProgram::ShaderFlag::STATIC);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// bind the vertex array buffer
	glBindVertexArray(terrain.GetMesh().GetVertexArrayBuffer());

	// bind the corresponding texture(s)
	terrain.GetMesh().GetMaterial().Activate(terrain.GetShader());

	// render
	glDrawElements(GL_TRIANGLES, terrain.GetMesh().GetIndices().size(), GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Engine::Renderer::Render(const Engine::Camera& camera, Engine::Volume& volume)
{
	// activate that abomination of a shader
	volume.GetShader().Activate();

	// find the locations of uniform variables in the shader and assign transform matrices to them
	glBindBuffer(GL_UNIFORM_BUFFER, Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::MVP_MATRICES));

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(volume.GetTransform()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetView()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(camera.GetProjection()));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// bind the vertex array buffer
	glBindVertexArray(volume.GetMesh().GetVertexArrayBuffer());

	// bind the corresponding texture(s)
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_3D, volume.GetTexture().GetTextureID());
	glUniform1i(ShaderProgram::GetCurrentShaderProgram().GetAttributeLocation(Engine::ShaderProgram::ShaderAttribute::VOLUME_MAP), 0);

	// render
	glDrawElements(GL_TRIANGLES, volume.GetMesh().GetIndices().size(), GL_UNSIGNED_INT, 0);
}

void Engine::Renderer::Render(const Engine::DirectionalLight& light, Engine::FrameBuffer& shadowBuffer, Engine::Actor& actor)
{
	// setup the viewport size to match the shadow frame buffer
	glViewport(0, 0, shadowBuffer.GetWidth(), shadowBuffer.GetHeight());

	// activate the shadow buffer
	shadowBuffer.Activate();
	glClear(GL_DEPTH_BUFFER_BIT);

	// activate the shadow mapping shader
	this->shadowMapShader.Activate();

	// find the locations of uniform variables in the shader and assign transform matrices to them
	glBindBuffer(GL_UNIFORM_BUFFER, Engine::ShaderProgram::GetUniformBuffer(Engine::ShaderProgram::UniformBuffer::MVP_MATRICES));

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(actor.GetTransform()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(light.GetView()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(light.GetProjection()));

	// go through all static meshes in actor's model and draw them
	for (size_t i = 0; i < actor.GetModel().GetStaticMeshes().size(); i++) {

		// bind the vertex array buffer
		glBindVertexArray(actor.GetModel().GetStaticMeshes().at(i).get()->GetVertexArrayBuffer());

		// render
		glDrawElements(GL_TRIANGLES, actor.GetModel().GetStaticMeshes().at(i).get()->GetIndices().size(), GL_UNSIGNED_INT, 0);
	}
	
	// unbind the uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// revert back to default frame buffer
	shadowBuffer.Deactivate();
}

void Engine::Renderer::SetColorDepth(int colorDepth)
{
	this->colorDepth = colorDepth;
}

unsigned short Engine::Renderer::GetColorDepth()
{
	return this->colorDepth;
}

unsigned int Engine::Renderer::QueryProperty(QueryableProperty property)
{
	switch (property)
	{
	case Engine::Renderer::QueryableProperty::MAX_TOTAL_TEXTURE_UNITS:
		return this->maxTotalTextureUnits;
	case Engine::Renderer::QueryableProperty::MAX_FRAGMENT_TEXTURE_UNITS:
		return this->maxFragmentTextureUnits;
	case Engine::Renderer::QueryableProperty::MAX_1D_TEXTURE_SIZE:
		return this->max1DTextureSize;
	case Engine::Renderer::QueryableProperty::MAX_2D_TEXTURE_SIZE:
		return this->max2DTextureSize;
	case Engine::Renderer::QueryableProperty::MAX_3D_TEXTURE_SIZE:
		return this->max3DTextureSize;
	case Engine::Renderer::QueryableProperty::MAX_CUBEMAP_TEXTURE_SIZE:
		return this->maxCubMapTextureSize;
	case Engine::Renderer::QueryableProperty::MAX_ARRAY_TEXTURE_LAYERS:
		return this->maxArrayTextureLayers;
	case Engine::Renderer::QueryableProperty::MAX_FRAMEBUFFER_WIDTH:
		return this->maxFrameBufferWidth;
	case Engine::Renderer::QueryableProperty::MAX_FRAMEBUFFER_HEIGHT:
		return this->maxFrameBufferHeight;
	case Engine::Renderer::QueryableProperty::MAX_RENDERBUFFER_SIZE:
		return this->maxRenderBufferSize;
	case Engine::Renderer::QueryableProperty::MAX_TEXTUREBUFFER_SIZE:
		return this->maxTextureBuferSize;
	case Engine::Renderer::QueryableProperty::MAX_FRAGMENT_INPUTS:
		return this->maxFragmentInputs;
	case Engine::Renderer::QueryableProperty::MAX_VERTEX_ATTRIBUTES:
		return this->maxVertexAttributes;
	case Engine::Renderer::QueryableProperty::MAX_VERTEX_OUTPUTS:
		return this->maxVertexOutputs;
	default:
		break;
	}
}

bool Engine::Renderer::IsExtensionSupported(std::string& extensionName)
{
	return glfwExtensionSupported(extensionName.c_str());
}
