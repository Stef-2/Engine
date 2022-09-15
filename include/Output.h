#pragma once

#include "filesystem"
#include "iostream"
#include "fstream"
#include "string"

namespace Engine
{

	// handles internal output buffers and file writing
	class Output
	{
	public:
		enum class WriteParameters : unsigned short
		{
			TEXT = 1 << 0,
			BINARY = 1 << 0,

			APPEND = 1 << 1,
			REPLACE = 1 << 1
		};

		static void WriteFile(void* data, size_t size, WriteParameters params, std::string name, std::string extension = "");


		// string buffer
		class Buffer
		{
		public:
			std::string_view GetData();
			std::string_view GetLastLine();

			Buffer& operator<< (std::string&);
			Buffer& operator<< (std::string_view);

		private:
			std::string buffer;
		};

		static Engine::Output::Buffer& Message();
		static Engine::Output::Buffer& Warning();
		static Engine::Output::Buffer& Error();

	private:
		static Buffer message;
		static Buffer warning;
		static Buffer error;
	};

}

