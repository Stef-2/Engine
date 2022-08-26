#include <Main.h>

Engine::Camera camera(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
Engine::Camera topOrh(0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);

double deltaTime = 0.0f;
double oldX = 1920.0 / 2.0;
double oldY = 1080.0 / 2.0;

float moveSpeed = 100.0f;

int main()
{
	//------------------------
	Engine::Window window(1920, 1080, "Engine", nullptr, nullptr, glm::ivec2(4, 6));
	Engine::Motor& engine = Engine::Motor::GetInstance();
	
	engine.Initialize();
	
	std::cout << "OpenGL version: " << window.GetGivenVersion() << std::endl;

	engine.SetWindow(window);

	Engine::Object::SetActiveObject(camera);
	camera.Setup(1.0f, engine.GetWindow().GetAspectRatio(), 0.1f, 10000, 45.0f);
	camera.SetUpDirection(glm::vec3(0.0f, 1.0f, 0.0f));
	Engine::InitializeCallbacks(&engine);

	// shadow buffer
	Engine::FrameBuffer shadowBuffer(window);
	shadowBuffer.AddAttachment(Engine::FrameBuffer::AttachmentType::DEPTH_ATTACHMENT);

	unsigned int vertexCount = 0;
	unsigned int triangleCount = 0;
	unsigned int numCulls = 0;
	bool check = {};

	double lastFrame = 0.0f;
	double lastTime = 0.0f;
	double frameMs = 0.0f;
	int nFrames = 0;
	int fps = 0;
	double glfwTestStart = glfwGetTime();

	// ===========================================================================
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) std::cout << "WTF!" << std::endl;
	FT_Face face;
	// =============================== test site =================================
	Engine::Image3D i3D(32, 32, 32);

	
	Engine::ShaderProgram writev(engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\volume.comp"));
	i3D.Compute(writev);

	Engine::ShaderProgram panelShader(engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\panel.vert"),
		engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\panel.frag"));

	unsigned long instancesSize = 1000;
	std::vector<Engine::SimpleInstance> sInstances;
	std::vector<Engine::ComplexInstance> cInstances;
	sInstances.reserve(instancesSize);
	cInstances.reserve(instancesSize);
	float r = 100.0f;
	#pragma omp simd
	for (int i = 0; i < instancesSize; i++)
	{
		cInstances.emplace_back(glm::translate(glm::mat4(1.0f), glm::vec3{ Engine::Random::Generate(-r, r), Engine::Random::Generate(-r, r), Engine::Random::Generate(-r, r) }));
		sInstances.emplace_back(glm::vec3{Engine::Random::Generate(-r, r), Engine::Random::Generate(-r, r), Engine::Random::Generate(-r, r) });
	}

	// ===========================================================================
	Engine::UserInterface::Panel panel1(0.0f, 1.0f, 2.0f, 1.0f);
	Engine::UserInterface::Panel panel2(0.0f, 0.0f, 1.0f, 1.0f);
	Engine::UserInterface::Panel panel3(0.0f, 0.0f, 1.2f, 0.7f);

	panel1.SetColor({ 0.3f, 0.1f, 1.0f, 1.0f });
	panel2.SetColor({ 0.5f, 0.4f, 0.3f, 1.0f });
	panel3.SetColor({ 0.1f, 1.0f, 0.2f, 1.0f });
	panel1.SetLayer(Engine::UserInterface::layers[8]);
	panel1.SetShader(panelShader);
	panel2.SetShader(panelShader);
	panel3.SetShader(panelShader);

	panel3.SetLayer(Engine::UserInterface::layers[10]);

	panel3.SetBorderColor(glm::vec4{ 1.0f, 0.0f, 0.3f, 1.0f });
	panel3.SetBorderThickness(5.0f);

	Engine::UIElementAnimation ani1{ {0.2f, 0.2f}, 1.0f };
	Engine::UIElementAnimation ani2 = -ani1;
	// ===========================================================================

	Engine::ShaderProgram instanced(engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\instanced.vert"),
		engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\instanced.frag"));

	Engine::ShaderProgram uber(engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\uber.vert"),
						engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\uber.geom"),
						engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\uber.frag"));

	Engine::ShaderProgram testStatic(engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\static.vert"),
							  engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\static.frag"));

	Engine::ShaderProgram testAnimated(engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\animated.vert"),
								engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\animated.frag"));

	Engine::ShaderProgram volume(engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\volume.vert"),
						  engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\volume.frag"));

	// ---------------------------------------------------------------------------------------------------------------------

	Engine::Actor obj1;
	obj1.SetShader(instanced);
	obj1.SetModel(Engine::Model(engine.GetFilePath(Engine::EngineFilePath::MODELS_PATH).append("\\barrel.obj")));
	obj1.GetModel().GetStaticMeshes().at(0)->SetMaterial(Engine::Material());
	obj1.GetModel().GetStaticMeshes().at(0)->GetMaterial().SetDiffuseMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\barrel_BaseColor.png"));
	obj1.GetModel().GetStaticMeshes().at(0)->GetMaterial().SetRoughnessMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\barrel_Roughness.png"));
	obj1.GetModel().GetStaticMeshes().at(0)->GetMaterial().SetMetallicMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\barrel_Metallic.png"));
	obj1.GetModel().GetStaticMeshes().at(0)->GetMaterial().SetNormalMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\barrel_Normal.png"));
	obj1.GetModel().GetStaticMeshes().at(0)->SetInstances(sInstances);
	obj1.GetModel().GetStaticMeshes().at(0)->SetInstances(cInstances);
	obj1.GetModel().GetStaticMeshes().at(0)->SetInstanceable(true, Engine::InstanceType::SIMPLE_INSTANCE);
	obj1.MoveAbsolute(100.0f, 0.0f, 0.0f);
	
	Engine::Actor obj2;
	obj2.SetShader(uber);
	obj2.SetModel(Engine::Model(engine.GetFilePath(Engine::EngineFilePath::MODELS_PATH).append("\\dancing_vampire.dae")));

	obj2.GetModel().GetAnimatedMeshes().at(0)->SetMaterial(Engine::Material());
	obj2.GetModel().GetAnimatedMeshes().at(0)->GetMaterial().SetDiffuseMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\Vampire_diffuse.png"));
	obj2.GetModel().GetAnimatedMeshes().at(0)->GetMaterial().SetRoughnessMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\Vampire_specular2.png"));
	obj2.GetModel().GetAnimatedMeshes().at(0)->GetMaterial().SetMetallicMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\black.png"));
	obj2.GetModel().GetAnimatedMeshes().at(0)->GetMaterial().SetNormalMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\Vampire_normal.png"));

	Engine::Actor obj3;
	obj3.SetShader(uber);
	obj3.SetModel(Engine::Model(engine.GetFilePath(Engine::EngineFilePath::MODELS_PATH).append("\\sphere.obj")));

	obj3.GetModel().GetStaticMeshes().at(0)->SetMaterial(Engine::Material());
	obj3.GetModel().GetStaticMeshes().at(0)->GetMaterial().SetDiffuseMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\rustediron2_basecolor.png"));
	obj3.GetModel().GetStaticMeshes().at(0)->GetMaterial().SetRoughnessMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\rustediron2_roughness.png"));
	obj3.GetModel().GetStaticMeshes().at(0)->GetMaterial().SetMetallicMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\rustediron2_metallic.png"));
	obj3.GetModel().GetStaticMeshes().at(0)->GetMaterial().SetNormalMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\rustediron2_normal.png"));
	obj3.MoveAbsolute(-60.0f, 0.0f, 0.0f);
	obj3.ScaleRelative(10.0f, 10.0f, 10.0f);

	Engine::Actor obj4;
	obj4.SetShader(volume);
	obj4.SetModel(Engine::Model(engine.GetFilePath(Engine::EngineFilePath::MODELS_PATH).append("\\sphere.obj")));
	//obj4.MoveAbsolute(0.0f, 100.0f, 0.0f);
	//obj4.ScaleAbsolute(100.0f, 10.0f, 100.0f);
	//std::cout << obj4.GetCounter();
	// ----------------------------------------------------------
	
	Engine::ShaderProgram skyBoxShader(engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\skybox.vert"),
								engine.GetFilePath(Engine::EngineFilePath::SHADERS_PATH).append("\\skybox.frag"));

	Engine::Skybox skyBox;
	skyBox.SetShader(skyBoxShader);

	std::string skyBoxTextures[6] = { engine.GetFilePath(Engine::EngineFilePath::SKYBOXES_PATH).append("\\day\\4.bmp"),
									  engine.GetFilePath(Engine::EngineFilePath::SKYBOXES_PATH).append("\\day\\2.bmp"),
									  engine.GetFilePath(Engine::EngineFilePath::SKYBOXES_PATH).append("\\day\\1.bmp"),
									  engine.GetFilePath(Engine::EngineFilePath::SKYBOXES_PATH).append("\\day\\3.bmp"),
									  engine.GetFilePath(Engine::EngineFilePath::SKYBOXES_PATH).append("\\day\\5.bmp"),
									  engine.GetFilePath(Engine::EngineFilePath::SKYBOXES_PATH).append("\\day\\6.bmp") };

	Engine::Texture2D skyBoxTex(skyBoxTextures);
	skyBox.SetTexture(skyBoxTex);

	skyBox.Setup();
	
	std::vector<Engine::Actor*> actors;
	actors.push_back(&obj2);
	/*
	Engine::Terrain terrain({ 5000, 5000.0 }, 0.01, engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\heightmap.png"));
	terrain.SetShader(uber);
	Engine::Material mat;
	terrain.AddMaterial(mat);
	terrain.GetMesh().SetMaterial(mat);
	terrain.GetMesh().GetMaterial().SetDiffuseMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\grey.png"));
	terrain.GetMesh().GetMaterial().SetRoughnessMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\white.png"));
	terrain.GetMesh().GetMaterial().SetMetallicMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\grey.png"));
	terrain.GetMesh().GetMaterial().SetNormalMap(engine.GetFilePath(Engine::EngineFilePath::TEXTURES_PATH).append("\\normal map.jpg"));
	std::cout << terrain.GetHeightMap().GetData() << std::endl;*/

	for (size_t i = 0; i < actors.size(); i++)
	{
		for (size_t j = 0; j < actors.at(i)->GetModel().GetAnimatedMeshes().size(); j++)
		{
			vertexCount += actors.at(i)->GetModel().GetAnimatedMeshes().at(j)->GetVertices().size();
			triangleCount += actors.at(i)->GetModel().GetAnimatedMeshes().at(j)->GetTriangles().size();
		}
	}

	engine.GetAnimator().Animate(obj2, obj2.GetModel().GetAnimatedMeshes().back()->GetAnimations().back().GetName());
	
	Engine::PointLight pointLight1;
	Engine::PointLight pointLight2;
	//Engine::PointLight pointLight3;
	//Engine::PointLight pointLight4;
	//Engine::PointLight pointLight5;

	Engine::SpotLight spotLight;
	Engine::DirectionalLight dirLight;
	Engine::AmbientLight ambientLight;

	pointLight1.SetIntensity(15000.0f);
	//pointLight1.SetColor({ 0.8f, 0.3f, 0.0f });
	pointLight2.SetIntensity(15000.0f);
	//pointLight2.SetColor({ 1.0f, 0.5f, 0.2f });
	//pointLight3.SetIntensity(50000.0f);
	//pointLight4.SetIntensity(50000.0f);
	//pointLight5.SetIntensity(50000.0f);

	//pointLight2.MoveAbsolute(-10.0f, -10.0f, -10.0f);
	//pointLight3.MoveAbsolute(+10.0f, -10.0f, +10.0f);
	//pointLight4.MoveAbsolute(-10.0f, +10.0f, -10.0f);
	//pointLight5.MoveAbsolute(+10.0f, +10.0f, +10.0f);
	
	spotLight.SetIntensity(15000.0f);
	
	dirLight.SetIntensity(2.0f);
	dirLight.SetOrientation({ 0.0f, 1.0f, 0.0f });
	dirLight.SetPosition({ 300.0f, 0.0f, 0.0f });
	//dirLight.SetColor({ 0.4f, 0.4f, 0.8f });
	ambientLight.SetIntensity(50.0f);

	// -----------------------------------------------
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	//glEnable(GL_FRAMEBUFFER_SRGBA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glDisable(GL_DEPTH_TEST);
	lastTime = glfwGetTime();
	// ===========================================================================
	// !!!---------------------------- main loop ------------------------------!!!
	// ===========================================================================
	while(!glfwWindowShouldClose(engine.GetWindow().GetGlWindow()))
	{
		// theres a whole bunch of overlap in here, gotta cleanup
		// obj1.MoveRelative(0.1f * deltaTime, 0.0f, 0.0f);
		// obj2.MoveRelative(-0.1f * deltaTime, 0.0f, 0.0f);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		nFrames++;

		if (currentFrame - lastTime >= 1.0f) {
			frameMs = 1000.0f / nFrames;
			fps = (CLOCKS_PER_SEC / deltaTime) / 1000;
			nFrames = 0;
			lastTime += 1.0f;
		}

		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_W) == GLFW_PRESS) {
			camera.MoveRelative(camera.GetForwardDirection(), moveSpeed * deltaTime);
		}
		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_S) == GLFW_PRESS) {
			camera.MoveRelative(camera.GetForwardDirection(), -moveSpeed * deltaTime);
		}
		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_A) == GLFW_PRESS) {
			camera.MoveRelative(camera.GetRightDirection(), -moveSpeed * deltaTime);
		}
		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_D) == GLFW_PRESS) {
			camera.MoveRelative(camera.GetRightDirection(), moveSpeed* deltaTime);
		}
		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
			camera.MoveRelative(camera.GetUpDirection(), moveSpeed * deltaTime);
		}
		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			camera.MoveRelative(camera.GetUpDirection(), -moveSpeed * deltaTime);
		}

		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
			glDisable(GL_CULL_FACE);
		}
		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_R) == GLFW_PRESS) {
			obj1.GetModel().GetStaticMeshes().at(0)->SetInstanceable(true, Engine::InstanceType::SIMPLE_INSTANCE);
			//panel3.SetVisiblity(false);
			engine.GetAnimator().Animate(panel3, ani1);
		}
		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_T) == GLFW_PRESS) {
			obj1.GetModel().GetStaticMeshes().at(0)->SetInstanceable(true, Engine::InstanceType::COMPLEX_INSTANCE);
			//panel3.SetVisiblity(true);
			engine.GetAnimator().Animate(panel3, ani2);
		}

		if (glfwGetKey(engine.GetWindow().GetGlWindow(), GLFW_KEY_E) == GLFW_PRESS) {
			engine.GetAnimator().Animate(obj2, obj2.GetModel().GetAnimatedMeshes().back()->GetAnimations().back().GetName());
			//glm::mat4 asd = glm::mat4_cast(glm::inverse(camera.GetOrientation())) * camera.GetView();
			/*glm::mat4 asd = camera.GetView();
			glm::vec3 inv = -asd[3] * glm::mat3(asd);
				for (size_t j = 0; j < 4; j++)
					std::cout << inv[j] << " ";
				std::cout << std::endl;
			std::cout << "--------------------" << std::endl;*/
			std::cout << Engine::ShaderProgram::DebugOutput();
			glEnable(GL_CULL_FACE);
			//std::cout << *glm::value_ptr(camera.GetView()[0][0]) << std::endl;
		}
		//asm("int $3");
		obj1.RotateRelative(0.0f, 15.0f * deltaTime, 0.0f);
		obj3.RotateRelative(0.0f, 15.0f * deltaTime, 0.0f);
		//obj4.MoveRelative(0.2f * deltaTime, 0.0f, 0.0f);
		//spotLight.RotateRelative(0.0f, 1.0f * deltaTime, 0.0f);
		engine.GetAnimator().UpdateAnimations();
		std::vector<Engine::Actor*> culled = engine.GetRenderer().FrustumCull(camera, actors);
		numCulls = actors.size() - culled.size();
		//engine.GetRenderer().Render(camera, actors);
		pointLight1.MoveRelative(0.0f, 10.0f * deltaTime, 0.0f);
		//engine.GetRenderer().Render(camera, pointLight1);
		//pointLight2.MoveRelative(0.0f, 10.0f * deltaTime, 0.0f);
		//engine.GetRenderer().Render(camera, pointLight2);
		//pointLight3.MoveRelative(0.0f, 10.0f * deltaTime, 0.0f);
		//engine.GetRenderer().Render(camera, pointLight3);
		//pointLight4.MoveRelative(0.0f, 10.0f * deltaTime, 0.0f);
		//engine.GetRenderer().Render(camera, pointLight4);
		//pointLight5.MoveRelative(0.0f, 10.0f * deltaTime, 0.0f);
		//engine.GetRenderer().Render(camera, pointLight5);

		//engine.GetRenderer().Render(camera, terrain);
		
		//engine.GetRenderer().Render(camera, obj1);
		//engine.GetRenderer().Render(camera, obj3);

		//engine.GetRenderer().Render(camera, obj1.GetModel().GetBoundingBox());
		//engine.GetRenderer().Render(camera, obj2.GetModel().GetBoundingBox());
		// camera.Draw(&obj1);
		// camera.Draw(&obj2);
		engine.GetRenderer().Render(camera, skyBox);

		// render shadows
		//engine.GetRenderer().Render(dirLight, shadowBuffer, obj1);

		//obj4.SetShader(wireFrame);
		engine.GetRenderer().Render(camera, obj1);
		engine.GetRenderer().Render(engine.GetUserInterface());
		//engine.GetRenderer().Render(camera, obj2);
		//engine.GetRenderer().Render(camera, obj3);
		//engine.GetRenderer().Render(camera, obj4);

		//obj4.MoveAbsolute(0.0f, glm::cos(glfwGetTime()), 0.0f);
		//glUniform1i(drawProgram->GetUniformLocation("sampler"), 0);
		//engine.GetRenderer().Render(camera, obj4);

		engine.GetWindow().SetTitle(std::string((const char*)(u8"Engine™ - Frame time: ") + std::to_string(frameMs) + " ms - FPS: " + std::to_string(fps) +
			" - Position: X: " + std::to_string(camera.GetPosition().x) + " - Y: " + std::to_string(camera.GetPosition().y) + " - Z: " + std::to_string(camera.GetPosition().z) +
			" - Rotation: X: " + std::to_string(camera.GetRotation().x) + " - Y: " + std::to_string(camera.GetRotation().y) + " - Z: " + std::to_string(camera.GetRotation().z) +
			" - number of culled objects: " + std::to_string(numCulls) + " - vertex count: " + std::to_string(vertexCount) + "- tri count: " + std::to_string(triangleCount)));

		glfwSwapBuffers(engine.GetWindow().GetGlWindow());
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		

		// obj1.Draw(&camera); // big guy (for obj2)
		// obj2.Draw(&camera); // small guy

		// camera.Draw(obj1.GetModel()->GetBoundingBox());
		// camera.Draw(obj2.GetModel()->GetBoundingBox());

		// obj1.RotateRelative(0.0f * deltaTime, -50.0f * deltaTime, 0.0f * deltaTime);
		// obj2.RotateRelative(0.0f * deltaTime, 50.0f * deltaTime, 0.0f * deltaTime );
		
		// camera.Draw(&octree);
	}

	glfwTerminate();
}

void Engine::ErrorCallback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

void Engine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		std::cout << "Window closure requested." << std::endl;
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void Engine::FrameBufferCallback(GLFWwindow* window, int width, int height)
{
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

void Engine::MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	double lastX = 1280/2, lastY = 720/2;
	double xoffset = xPos - lastX;
	double yoffset = lastY - yPos;
	double yaw = 0, pitch = 0;
	lastX = xPos;
	lastY = yPos;

	double sensitivity = 1;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw   += xoffset;
	pitch += yoffset;

	double difX = (xPos - oldX) * (sensitivity);
	double difY = (yPos - oldY) * (sensitivity);

	oldX = xPos;
	oldY = yPos;

	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 rot = glm::vec3(difY, difX, 0.0f);

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	// std::cout << difX << " - " << difY << std::endl;
	// camera.RotateAbsolute(direction.x, direction.y, direction.z);
	camera.RotateRelative(rot.x, rot.y, 0.0f);
}

void Engine::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	// camera.SetFov(camera.GetFov() - (float)yOffset);
}

int Engine::Initialize()
{
	//  Initialize the library
	if (!glfwInit())
		return 0;

	return 1;
}

void Engine::InitializeCallbacks(Engine::Motor* Motor)
{
	// setup glfw callbacks
	glfwSetErrorCallback(ErrorCallback);
	glfwSetKeyCallback(Motor->GetWindow().GetGlWindow(), KeyCallback);
	glfwSetFramebufferSizeCallback(Motor->GetWindow().GetGlWindow(), FrameBufferCallback);
	glfwSetCursorPosCallback(Motor->GetWindow().GetGlWindow(), MouseCallback);
	glfwSetScrollCallback(Motor->GetWindow().GetGlWindow(), ScrollCallback);
}
