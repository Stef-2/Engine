#ifndef WINDOW_H
#define WINDOW_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "glfw3.h"
#include "glm/vec2.hpp"
#include "memory"
#include "iostream"
#include "string"

namespace Engine
{
	struct OpenGLVersion
	{
		using OpenGLVersionNumber = unsigned short;

		OpenGLVersionNumber major;
		OpenGLVersionNumber minor;
	};

	// GLFWwindow wrapper class
	class Window
	{
	public:

		using ScreenDimension = unsigned int;

		struct WindowDimensions
		{
			ScreenDimension width;
			ScreenDimension height;
		};

		Window(WindowDimensions, const char* title, OpenGLVersion version, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
		Window(WindowDimensions, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
		Window(WindowDimensions, const char* title);
		Window();

		Window(Window&&) = default;
		Window& operator= (Window&&) = default;

		Window(Window&) = default;
		Window& operator= (Window&) = default;

		~Window();




		GLFWwindow* GetGlWindow();
		GLFWwindow* GetShared();
		GLFWmonitor* GetMonitor();
		std::string_view GetTitle();
		double GetAspectRatio();
		OpenGLVersion GetRequestedVersion();
		std::string_view GetGivenVersion();
		WindowDimensions& GetDimensions();

		void SetWindowSize(WindowDimensions);
		void SetWidth(ScreenDimension);
		void SetHeight(ScreenDimension);
		
		void SetTitle(std::string newTitle);
		void SetTitle(std::u8string newTitle);

		void SetMonitor(GLFWmonitor* monitor);
		void SetShare(GLFWwindow* share);
		void SetVersion(OpenGLVersion version);

		void SetFullScreen(bool);
		bool GetFullScreen();

		void SetShouldClose(bool);
		bool GetShouldClose();

		void Initialize();

	private:
		// glfw's own window struct
		GLFWwindow* openGlWindow;

		// slots for windows with which we can share content
		GLFWwindow* share;

		// monitor associated with the window
		GLFWmonitor* monitor;

		// the requested OpenGL version
		OpenGLVersion openGlVersion;

		// OpenGL isn't guaranted to give us the version we asked for;
		// we'll store the one we're given here
		std::string openGlGivenVersion;

		WindowDimensions dimensions;

		// OpenGL window title
		std::string title;

		bool fullscreen;

		static Window* currentWindow;
	};

}

#endif //  WINDOW_H
