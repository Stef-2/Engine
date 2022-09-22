#pragma once

#include "glfw3.h"

#ifndef STB_IMAGE_IMPLEMENTATION
	#include "stb_image.h"
	#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Output.h"

#include "functional"
#include "variant"
#include "vector"
#include "array"
#include "map"

namespace Engine
{

	// static input handling class, subsystem of Engine::Motor
	class Input
	{
	public:
		// singleton instance provider
		static Input& GetInput();

		// possible input modes
		enum class InputMode
		{
			GAME_INPUT,
			USER_INTERFACE_INPUT,
			TEXT_INPUT
		};

		// keyboard input ===========================================================

		enum class KeyboardKey : int
		{
			KEY_UNKNOWN				= GLFW_KEY_UNKNOWN,
			KEY_SPACE				= GLFW_KEY_SPACE,
			KEY_APOSTROPHE			= GLFW_KEY_APOSTROPHE,
			KEY_COMMA				= GLFW_KEY_COMMA,
			KEY_MINUS				= GLFW_KEY_MINUS,
			KEY_PERIOD				= GLFW_KEY_PERIOD,
			KEY_SLASH				= GLFW_KEY_SLASH,
			KEY_0					= GLFW_KEY_0,
			KEY_1					= GLFW_KEY_1,
			KEY_2					= GLFW_KEY_2,
			KEY_3					= GLFW_KEY_3,
			KEY_4					= GLFW_KEY_4,
			KEY_5					= GLFW_KEY_5,
			KEY_6					= GLFW_KEY_6,
			KEY_7					= GLFW_KEY_7,
			KEY_8					= GLFW_KEY_8,
			KEY_9					= GLFW_KEY_9,
			KEY_SEMICOLON			= GLFW_KEY_SEMICOLON,
			KEY_EQUAL				= GLFW_KEY_EQUAL,
			KEY_A					= GLFW_KEY_A,
			KEY_B					= GLFW_KEY_B,
			KEY_C					= GLFW_KEY_C,
			KEY_D					= GLFW_KEY_D,
			KEY_E					= GLFW_KEY_E,
			KEY_F					= GLFW_KEY_F,
			KEY_G					= GLFW_KEY_G,
			KEY_H					= GLFW_KEY_H,
			KEY_I					= GLFW_KEY_I,
			KEY_J					= GLFW_KEY_J,
			KEY_K					= GLFW_KEY_K,
			KEY_L					= GLFW_KEY_L,
			KEY_M					= GLFW_KEY_M,
			KEY_N					= GLFW_KEY_N,
			KEY_O					= GLFW_KEY_O,
			KEY_P					= GLFW_KEY_P,
			KEY_Q					= GLFW_KEY_Q,
			KEY_R					= GLFW_KEY_R,
			KEY_S					= GLFW_KEY_S,
			KEY_T					= GLFW_KEY_T,
			KEY_U					= GLFW_KEY_U,
			KEY_V					= GLFW_KEY_V,
			KEY_W					= GLFW_KEY_W,
			KEY_X					= GLFW_KEY_X,
			KEY_Y					= GLFW_KEY_Y,
			KEY_Z					= GLFW_KEY_Z,
			KEY_LEFT_BRACKET		= GLFW_KEY_LEFT_BRACKET,
			KEY_BACKSLASH			= GLFW_KEY_BACKSLASH,
			KEY_RIGHT_BRACKET		= GLFW_KEY_RIGHT_BRACKET,
			KEY_GRAVE_ACCENT		= GLFW_KEY_GRAVE_ACCENT,
			KEY_WORLD_1				= GLFW_KEY_WORLD_1,
			KEY_WORLD_2				= GLFW_KEY_WORLD_2,
			KEY_ESCAPE				= GLFW_KEY_ESCAPE,
			KEY_ENTER				= GLFW_KEY_ENTER,
			KEY_TAB					= GLFW_KEY_TAB,
			KEY_BACKSPACE			= GLFW_KEY_BACKSPACE,
			KEY_INSERT				= GLFW_KEY_INSERT,
			KEY_DELETE				= GLFW_KEY_DELETE,
			KEY_RIGHT				= GLFW_KEY_RIGHT,
			KEY_LEFT				= GLFW_KEY_LEFT,
			KEY_DOWN				= GLFW_KEY_DOWN,
			KEY_UP					= GLFW_KEY_UP,
			KEY_PAGE_UP				= GLFW_KEY_PAGE_UP,
			KEY_PAGE_DOWN			= GLFW_KEY_PAGE_DOWN,
			KEY_HOME				= GLFW_KEY_HOME,
			KEY_END					= GLFW_KEY_END,
			KEY_CAPS_LOCK			= GLFW_KEY_CAPS_LOCK,
			KEY_SCROLL_LOCK			= GLFW_KEY_SCROLL_LOCK,
			KEY_NUM_LOCK			= GLFW_KEY_NUM_LOCK,
			KEY_PRINT_SCREEN		= GLFW_KEY_PRINT_SCREEN,
			KEY_PAUSE				= GLFW_KEY_PAUSE,
			KEY_F1					= GLFW_KEY_F1,
			KEY_F2					= GLFW_KEY_F2,
			KEY_F3					= GLFW_KEY_F3,
			KEY_F4					= GLFW_KEY_F4,
			KEY_F5					= GLFW_KEY_F5,
			KEY_F6					= GLFW_KEY_F6,
			KEY_F7					= GLFW_KEY_F7,
			KEY_F8					= GLFW_KEY_F8,
			KEY_F9					= GLFW_KEY_F9,
			KEY_F10					= GLFW_KEY_F10,
			KEY_F11					= GLFW_KEY_F11,
			KEY_F12					= GLFW_KEY_F12,
			KEY_F13					= GLFW_KEY_F13,
			KEY_F14					= GLFW_KEY_F14,
			KEY_F15					= GLFW_KEY_F15,
			KEY_F16					= GLFW_KEY_F16,
			KEY_F17					= GLFW_KEY_F17,
			KEY_F18					= GLFW_KEY_F18,
			KEY_F19					= GLFW_KEY_F19,
			KEY_F20					= GLFW_KEY_F20,
			KEY_F21					= GLFW_KEY_F21,
			KEY_F22					= GLFW_KEY_F22,
			KEY_F23					= GLFW_KEY_F23,
			KEY_F24					= GLFW_KEY_F24,
			KEY_F25					= GLFW_KEY_F25,
			KEY_KP_0				= GLFW_KEY_KP_0,
			KEY_KP_1				= GLFW_KEY_KP_1,
			KEY_KP_2				= GLFW_KEY_KP_2,
			KEY_KP_3				= GLFW_KEY_KP_3,
			KEY_KP_4				= GLFW_KEY_KP_4,
			KEY_KP_5				= GLFW_KEY_KP_5,
			KEY_KP_6				= GLFW_KEY_KP_6,
			KEY_KP_7				= GLFW_KEY_KP_7,
			KEY_KP_8				= GLFW_KEY_KP_8,
			KEY_KP_9				= GLFW_KEY_KP_9,
			KEY_KP_DECIMAL			= GLFW_KEY_KP_DECIMAL,
			KEY_KP_DIVIDE			= GLFW_KEY_KP_DIVIDE,
			KEY_KP_MULTIPLY			= GLFW_KEY_KP_MULTIPLY,
			KEY_KP_SUBTRACT			= GLFW_KEY_KP_SUBTRACT,
			KEY_KP_ADD				= GLFW_KEY_KP_ADD,
			KEY_KP_ENTER			= GLFW_KEY_KP_ENTER,
			KEY_KP_EQUAL			= GLFW_KEY_KP_EQUAL,
			KEY_LEFT_SHIFT			= GLFW_KEY_LEFT_SHIFT,
			KEY_LEFT_CONTROL		= GLFW_KEY_LEFT_CONTROL,
			KEY_LEFT_ALT			= GLFW_KEY_LEFT_ALT,
			KEY_LEFT_SUPER			= GLFW_KEY_LEFT_SUPER,
			KEY_RIGHT_SHIFT			= GLFW_KEY_RIGHT_SHIFT,
			KEY_RIGHT_CONTROL		= GLFW_KEY_RIGHT_CONTROL,
			KEY_RIGHT_ALT			= GLFW_KEY_RIGHT_ALT,
			KEY_RIGHT_SUPER			= GLFW_KEY_RIGHT_SUPER,
			KEY_MENU				= GLFW_KEY_MENU
		};

		enum class MouseKey : int
		{
			MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1,
			MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2,
			MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3,
			MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4,
			MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5,
			MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6,
			MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7,
			MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8,
			MOUSE_BUTTON_LAST = GLFW_MOUSE_BUTTON_8,
			MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_1,
			MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_2,
			MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_3
		};

		// typedef combining keyboard and mouse keys into one
		using PhysicalKey = std::variant<KeyboardKey, MouseKey>;
		
		// keyboard modifier keys
		enum class ModifierKey : int
		{
			NONE = 0x0000'0000,
			SHIFT = GLFW_MOD_SHIFT,
			CONTROL = GLFW_MOD_CONTROL,
			ALT = GLFW_MOD_ALT,
			SUPER = GLFW_MOD_SUPER,
			CAPS_LOCK = GLFW_MOD_CAPS_LOCK,
			NUM_LOCK = GLFW_MOD_NUM_LOCK,
			DEFAULT = NONE
		};

		// possible key states
		enum class KeyState : int
		{
			PRESSED = GLFW_PRESS,
			RELEASED = GLFW_RELEASE,
			HELD = GLFW_REPEAT,
			DEFAULT = PRESSED
		};

		// full key input, combining a key, modifier(s), and state which should trigger it
		// allows initialization from just a key, with modifiers defaulting to NONE and state to PRESSED
		struct KeyInput
		{
			// make constructible from PhysicalKey
			inline KeyInput(PhysicalKey& key)
				: key(key), modifiers(ModifierKey::DEFAULT), state(KeyState::DEFAULT)
				{};

			// default constructor from physical key, usually a mouthful to declare properly
			inline KeyInput(PhysicalKey& key, ModifierKey& modifier, KeyState& state)
				: key(key), modifiers(modifier), state(state)
				{};

			// make constructible from keyboard and mouse keys for convenience
			inline KeyInput(KeyboardKey& key)
				: key{key}, modifiers(ModifierKey::DEFAULT), state(KeyState::DEFAULT)
				{};

			inline KeyInput(MouseKey& key)
				: key {key}, modifiers(ModifierKey::DEFAULT), state(KeyState::DEFAULT)
				{};

			// comparison operator needed for std::map
			friend bool operator<(const KeyInput& lhs, const KeyInput& rhs) {return lhs.key < rhs.key;};

			const PhysicalKey key;
			const ModifierKey modifiers;
			const KeyState state;
		};

		// indexed std::function<>() wrapper, plain ones are indistinguishable from each other
		struct Action
		{
			// make constructible from std::function&
			inline Action(std::function<void()>& action)
				: action{action}, id(count)
				{ count++; };

			// and from std::function&&
			inline Action(std::function<void()>&& action)
				: action {action}, id(count)
				{ count++; };

			// copy and move constructors needed for std::vector and other containers
			Action(Action&&) noexcept = default;
			Action& operator=(Action&&) noexcept = default;
			
			Action(const Action&) = default;
			Action& operator=(const Action&) = default;

			// comparison operators needed for std::map
			inline friend bool operator==(const Action& lhs, const Action& rhs)
				{return lhs.id == rhs.id;};

			inline friend bool operator<(const Action& lhs, const Action& rhs)
				{return lhs.id < rhs.id;}

			// internal function access
			inline std::function<void()>& GetAction()
				{ return action; };

			inline void operator()()
				{ action(); }

		private:
			std::function<void()> action;
			unsigned int id;

			// global counter
			inline static unsigned long count = 0u;
		};

		// input event handling 

		// parse hardware inputs
		void ParseEvents();
		// wait for an input indefinitely
		void WaitForEvent();
		// wait for an input for x seconds, trigger after timeout
		void WaitForEvent(float seconds);
		// trigger an event that does nothing, can be used to wake up sleeping threads
		void TriggerNullEvent();

		// convert a physical key into a system specific scancode
		int GetKeyScanCode(PhysicalKey);
		// convert a physical key into its platform specific name
		std::string GetKeyName(PhysicalKey);

		// last pressed keys
		KeyboardKey GetLastKeyboardKey();
		MouseKey GetLastMouseKey();

		// last recorded key state
		KeyState GetKeyState(PhysicalKey);

		// input mode
		void SetInputMode(InputMode);
		InputMode GetCurrentInputMode();

		// enable and disable num and caps lock
		void SetLockKeys(bool);
		bool GetLockKeys();

		// key mapping
		bool IsKeyMapped(KeyInput);
		unsigned int GetNumBoundActions(KeyInput);
		void MapKey(KeyInput, Action&);
		void UnmapActionFromKey(KeyInput, Action&);
		void UnmapKey(KeyInput);

		// mouse input ==============================================================

		class Cursor
		{
			using dimension = unsigned int;

		public:
			// predefined standard set of window cursor shapes
			enum class StandardCursorShapes : int
			{
				ARROW_CURSOR = GLFW_ARROW_CURSOR,
				IBEAM_CURSOR = GLFW_IBEAM_CURSOR,
				CROSSHAIR_CUROSR = GLFW_CROSSHAIR_CURSOR,
				HAND_CURSOR = GLFW_HAND_CURSOR,
				HORIZONTAL_RESIZE_CURSOR = GLFW_HRESIZE_CURSOR,
				VERTICAL_RESIZE_CURSOR = GLFW_VRESIZE_CURSOR,
				DEFAULT = ARROW_CURSOR
			};

			enum class CursorMode : int
			{
				CURSOR_NORMAL = GLFW_CURSOR_NORMAL,
				CURSOR_HIDDEN = GLFW_CURSOR_HIDDEN,
				CURSOR_DISABLED = GLFW_CURSOR_DISABLED,
				DEFAULT = CURSOR_NORMAL
			};

			// custom cursor images class
			class CursorImage
			{
			public:
				CursorImage(std::string_view filePath);

				GLFWimage& GetGFLFWImage();
				void SetGLFWImage(GLFWimage&);

			private:
				GLFWimage image;
			};

			// standard cursor
			Cursor();
			Cursor(dimension xOffset, dimension yOffset);
			Cursor(CursorImage&, dimension xOffset, dimension yOffset);

			void SetImage(CursorImage&);
			CursorImage& GetImage();

			void SetGLFWCursor(GLFWcursor*);
			GLFWcursor* GetGLFWCursor();

			void SetCursorMode(CursorMode);
			CursorMode GetCursorMode();

			void SetCursorPosition(double x, double y);
			std::array<double, 2> GetCursorPosition();

			void SetRawMode(bool);
			bool GetRawMode();

		private:
			std::unique_ptr<CursorImage> image;

			GLFWcursor* cursor;
			GLFWwindow* window;

			CursorMode mode;
		};

		// input callbacks
		// being a C library with no knowledge of objects or this->pointer,
		// glfw can only accept static or fully global functions as valid callbacks
		static void InitializeCallbacks();

		// keyboard input handling
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		// mouse scroll handling
		static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		// mouse movement handling
		static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
		// text input handling
		static void CharacterCallback(GLFWwindow* window, unsigned int codepoint);

	private:
		// hide the constructors
		Input(GLFWwindow&);
		Input(const Input&) = delete;
		void operator=(const Input&) = delete;

		std::map<KeyInput, std::vector<Action>> keyMapping;

		GLFWwindow* window;
		Cursor cursor;
		InputMode currentInputMode;

		KeyboardKey lastKeyboardKey;
		MouseKey lastMouseKey;
	};
}

