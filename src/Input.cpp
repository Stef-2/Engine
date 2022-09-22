#include "Input.h"
/*
Engine::Input::InputMode Engine::Input::currentInputMode = Engine::Input::InputMode::USER_INTERFACE_INPUT;
std::map<Engine::Input::KeyInput, std::vector<Engine::Input::Action>> Engine::Input::keyMapping {};
Engine::Input::KeyboardKey Engine::Input::lastKeyboardKey {Engine::Input::KeyboardKey::KEY_UNKNOWN};
Engine::Input::MouseKey Engine::Input::lastMouseKey {Engine::Input::MouseKey::MOUSE_BUTTON_LAST};
GLFWwindow* Engine::Input::window = {};
Engine::Input::Cursor Engine::Input::cursor{};
*/
Engine::Input& Engine::Input::GetInput()
{
	static Input input;

	return input;
}

void Engine::Input::ParseEvents()
{
	glfwPollEvents();
}

void Engine::Input::WaitForEvent()
{
	glfwWaitEvents();
}

void Engine::Input::TriggerNullEvent()
{
	glfwPostEmptyEvent();
}

int Engine::Input::GetKeyScanCode(PhysicalKey key)
{
	return glfwGetKeyScancode(std::to_underlying(std::get<0>(key)));
}

std::string Engine::Input::GetKeyName(PhysicalKey key)
{
	return glfwGetKeyName(std::to_underlying(std::get<0>(key)), 0);
}

Engine::Input::KeyboardKey Engine::Input::GetLastKeyboardKey()
{
	return Engine::Input::lastKeyboardKey;
}

Engine::Input::MouseKey Engine::Input::GetLastMouseKey()
{
	return Engine::Input::lastMouseKey;
}

Engine::Input::KeyState Engine::Input::GetKeyState(PhysicalKey key)
{
	return static_cast<KeyState>(glfwGetKey(Engine::Input::window, std::to_underlying(std::get<0>(key))));
}

void Engine::Input::WaitForEvent(float seconds)
{
	glfwWaitEventsTimeout(seconds);
}

void Engine::Input::SetInputMode(InputMode mode)
{
	Engine::Input::currentInputMode = mode;
}

Engine::Input::InputMode Engine::Input::GetCurrentInputMode()
{
	return Engine::Input::currentInputMode;
}

void Engine::Input::SetLockKeys(bool value)
{
	glfwSetInputMode(Engine::Input::window, GLFW_LOCK_KEY_MODS, value);
}

bool Engine::Input::GetLockKeys()
{
	return glfwGetInputMode(Engine::Input::window, GLFW_LOCK_KEY_MODS);
}

bool Engine::Input::IsKeyMapped(KeyInput key)
{
	return Engine::Input::keyMapping.contains(key);
}

void Engine::Input::MapKey(KeyInput key, Action& action)
{
	// if the key is already mapped, just add the action to the stack
	if (Input::IsKeyMapped(key))
		Engine::Input::keyMapping.at(key).push_back(action);

	// otherwise, add a new key => actions pair
	else
		Engine::Input::keyMapping.emplace(key, std::vector<Action>{action});
}

void Engine::Input::UnmapActionFromKey(KeyInput key, Action& action)
{
	// check if the key is mapped
	if (Engine::Input::IsKeyMapped(key))
	{
		auto& actions = Engine::Input::keyMapping.at(key);

		// check if the action exists
		auto itterator = std::find(actions.begin(), actions.end(), action);

		if (itterator != actions.end())
			// remove it
			actions.erase(itterator);
	}
}

void Engine::Input::UnmapKey(KeyInput key)
{
	Engine::Input::keyMapping.erase(key);
}

unsigned int Engine::Input::GetNumBoundActions(KeyInput key)
{
	if (!Engine::Input::keyMapping.contains(key))
		return 0;

	return Engine::Input::keyMapping.count(key);
}

Engine::Input::Cursor::CursorImage::CursorImage(std::string_view filePath)
{
	// stbi tends to flip images for some reason
	stbi_set_flip_vertically_on_load(true);

	// load data
	int numChannels = 0xDEAD;

	this->image.pixels = stbi_load(filePath.data(), &this->image.width, &this->image.height, &numChannels, 0);
	
	if (numChannels != 4)
		Engine::Output::Error() << "attempting to create a cursor image using: " << filePath.data() << ", which has: " << std::to_string(numChannels) << " pixel channels";
}

GLFWimage& Engine::Input::Cursor::CursorImage::GetGFLFWImage()
{
	return this->image;
}

void Engine::Input::Cursor::CursorImage::SetGLFWImage(GLFWimage& image)
{
	this->image = image;
}

Engine::Input::Cursor::Cursor()
	: image{nullptr},
	cursor{glfwCreateStandardCursor
	(std::to_underlying(Engine::Input::Cursor::StandardCursorShapes::ARROW_CURSOR))}
{
	// check if window hook is set
	auto window = Engine::Input::window;
	if (!window)
		return;

	int width = 0;
	int height = 0;

	// set cursor in the middle of the screen
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2.0, height / 2.0);
}

void Engine::Input::Cursor::SetImage(CursorImage& image)
{
	this->image.reset(&image);
}

Engine::Input::Cursor::CursorImage& Engine::Input::Cursor::GetImage()
{
	return *this->image;
}

void Engine::Input::Cursor::SetGLFWCursor(GLFWcursor* cursor)
{
	this->cursor = cursor;
}

GLFWcursor* Engine::Input::Cursor::GetGLFWCursor()
{
	return this->cursor;
}

void Engine::Input::Cursor::SetCursorMode(CursorMode mode)
{
	this->mode = mode;
	glfwSetInputMode(Engine::Input::window, GLFW_CURSOR, std::to_underlying(mode));
}

Engine::Input::Cursor::CursorMode Engine::Input::Cursor::GetCursorMode()
{
	return this->mode;
}

void Engine::Input::Cursor::SetCursorPosition(double x, double y)
{
	glfwSetCursorPos(Engine::Input::window, x, y);
}

std::array<double, 2> Engine::Input::Cursor::GetCursorPosition()
{
	std::array<double, 2> position;

	glfwGetCursorPos(Engine::Input::window, &position[0], &position[1]);

	return position;
}

void Engine::Input::Cursor::SetRawMode(bool value)
{
	glfwSetInputMode(Engine::Input::window, GLFW_RAW_MOUSE_MOTION, value);
}

bool Engine::Input::Cursor::GetRawMode()
{
	return glfwGetInputMode(Engine::Input::window, GLFW_RAW_MOUSE_MOTION);
}

Engine::Input::Cursor::Cursor(CursorImage& image, dimension xOffset, dimension yOffset)
	: image{&image}, cursor{glfwCreateCursor(&image.GetGFLFWImage(), xOffset, yOffset)}
{

}

Engine::Input::Cursor::Cursor(dimension xOffset, dimension yOffset)
	: image{nullptr},
	cursor{glfwCreateCursor(nullptr, xOffset, yOffset)}
{

}
