#include "Window.h"

Engine::Window::Window()
{
	this->dimensions = {};
	this->title = {};
	this->openGlVersion = {};
	this->monitor = {};
	this->share = {};
	this->openGlWindow = {};
}

Engine::Window::Window(WindowDimensions dimensions, const char* title, OpenGLVersion version, GLFWmonitor* monitor, GLFWwindow* share)
: dimensions(dimensions), title(const_cast<char*>(title)), openGlWindow(nullptr), monitor(monitor), share(share),
  openGlVersion(version), fullscreen(false)
{
	Initialize();
}

Engine::Window::Window(WindowDimensions dimensions, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
: dimensions(dimensions), title(const_cast<char*>(title)), openGlWindow(nullptr), monitor(monitor),
  share(share), fullscreen(false)
{
	this->openGlVersion = {4, 6};
	Initialize();
}

Engine::Window::Window(WindowDimensions dimensions, const char* title)
	: dimensions(dimensions), title(const_cast<char*>(title)), openGlWindow(nullptr), monitor(nullptr), share(nullptr),
	  fullscreen(false)
{
	this->openGlVersion = {4, 6};
	Initialize();
}

void Engine::Window::Initialize()
{
	// initialize glfw
	if (!glfwInit()) {
		std::cerr << "Glfw could not be initialized." << std::endl;
		abort();
	}

	// check if the window has already been initialized
	if (!this->openGlWindow)
	{
		// check if all the required parameters have been set up
		if (this->dimensions.width && this->dimensions.height && this->openGlVersion.major)
		{
			// fullscreen
			//this->monitor = glfwGetPrimaryMonitor();

			// politely tell OpenGL which version of it we would like to use
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->openGlVersion.major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->openGlVersion.minor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
			//glfwWindowHint(GLFW_SAMPLES, 4);
			this->openGlWindow = glfwCreateWindow(this->dimensions.width, this->dimensions.height, title.c_str(), monitor, share);
			
			glfwMaximizeWindow(openGlWindow);
			//glfwSetWindowPos(this->openGlWindow, 0, 0);
			glfwMakeContextCurrent(openGlWindow);
			glfwSetInputMode(openGlWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSwapInterval(0);
			gladLoadGL();

			int width, height;
			glfwGetWindowSize(this->openGlWindow, &width, &height);
			this->SetWindowSize(this->dimensions);
			std::cout << "size: " << width << " " << height << '\n';
			// OpenGL isn't required to give us the requested version, we'll store whatever it gave us
			this->openGlGivenVersion = (const char*)(glGetString(GL_VERSION));

			if (!openGlWindow || !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cerr << "OpenGL could not create a window context." << std::endl;
				glfwTerminate();
			}
		}
		else {
			std::cerr << "Some or all of the parameters needed for OpenGL Window creation are missing." << std::endl;
			glfwTerminate();
		}
	}
	else
		std::cerr << "Window already initilized." << std::endl;
}

Engine::Window::~Window()
{
	if(!openGlWindow)
		glfwDestroyWindow(openGlWindow);
	if(!share)
		glfwDestroyWindow(share);
	if(!monitor)
		delete monitor;
}

double Engine::Window::GetAspectRatio()
{
	return static_cast<double>(this->dimensions.width) / static_cast<float>(this->dimensions.height);
}

void Engine::Window::SetTitle(std::string newTitle)
{
	this->title = newTitle;
	glfwSetWindowTitle(this->openGlWindow, title.c_str());
}

void Engine::Window::SetTitle(std::u8string newTitle)
{
	this->title = std::string(newTitle.begin(), newTitle.end());
	glfwSetWindowTitle(this->openGlWindow, title.c_str());
}

void Engine::Window::SetWidth(Engine::Window::ScreenDimension dimension)
{
	this->dimensions.width = dimension;
	glfwSetWindowSize(this->openGlWindow, this->dimensions.width, this->dimensions.height);
}

void Engine::Window::SetHeight(Engine::Window::ScreenDimension dimension)
{
	this->dimensions.height = dimension;
	glfwSetWindowSize(this->openGlWindow, this->dimensions.width, this->dimensions.height);
}

void Engine::Window::SetVersion(OpenGLVersion version)
{
	this->openGlVersion = version;
}

void Engine::Window::SetFullScreen(bool value)
{
	this->fullscreen = value;

	this->monitor = (this->fullscreen) ? glfwGetPrimaryMonitor() : nullptr;
}

bool Engine::Window::GetFullScreen()
{
	return this->fullscreen;
}

void Engine::Window::SetShouldClose(bool value)
{
	glfwSetWindowShouldClose(this->openGlWindow, value);
}

bool Engine::Window::GetShouldClose()
{
	return glfwWindowShouldClose(this->openGlWindow);
}

void Engine::Window::SetMonitor(GLFWmonitor* monitor)
{
	this->monitor = monitor;
}

void Engine::Window::SetShare(GLFWwindow* window)
{
	this->share = window;
}

GLFWwindow* Engine::Window::GetGlWindow()
{
	return this->openGlWindow;
}

Engine::OpenGLVersion Engine::Window::GetRequestedVersion()
{
	return this->openGlVersion;
}

std::string_view Engine::Window::GetGivenVersion()
{
	return this->openGlGivenVersion;
}

Engine::Window::WindowDimensions& Engine::Window::GetDimensions()
{
	return this->dimensions;
}

void Engine::Window::SetWindowSize(Engine::Window::WindowDimensions dimensions)
{
	this->dimensions = dimensions;
	glfwSetWindowSize(this->openGlWindow, this->dimensions.width, this->dimensions.height);
}

std::string_view Engine::Window::GetTitle()
{
	return this->title;
}

GLFWmonitor* Engine::Window::GetMonitor()
{
	return this->monitor;
}

GLFWwindow* Engine::Window::GetShared()
{
	return this->share;
}
