#include "Output.h"

Engine::Output::Buffer Engine::Output::information;
Engine::Output::Buffer Engine::Output::warning;
Engine::Output::Buffer Engine::Output::error;

void Engine::Output::WriteFile(void* data, size_t size, WriteParameters params, std::string name, std::string extension)
{
	// check data validity
	if (!data || !size)
		return;

	if (extension.empty())
		if (std::to_underlying(params) & std::to_underlying(Output::WriteParameters::BINARY))
			extension = "bin";
		else
			extension = "txt";

	auto openMode = std::ios::in | std::ios::out;

	if (std::to_underlying(params) & std::to_underlying(Output::WriteParameters::BINARY))
		openMode |= std::ios::binary;

	if (std::to_underlying(params) & std::to_underlying(Output::WriteParameters::REPLACE))
		openMode |= std::ios::trunc;

	if (std::to_underlying(params) & std::to_underlying(Output::WriteParameters::APPEND))
		openMode |= std::ios::app;

	std::fstream stream(name + extension, openMode);

	if (stream.is_open())
		stream.write(reinterpret_cast<char*>(data), size);

	stream.close();
}

Engine::Output::Buffer& Engine::Output::Information()
{
	return Engine::Output::information;
}

Engine::Output::Buffer& Engine::Output::Warning()
{
	return Engine::Output::warning;
}

Engine::Output::Buffer& Engine::Output::Error()
{
	return Engine::Output::error;
}

std::string_view Engine::Output::Buffer::GetData()
{
	return this->buffer;
}

std::string_view Engine::Output::Buffer::GetLastLine()
{
	std::size_t position = this->buffer.find_last_of('\n', this->buffer.length() - 2);

	std::string result = this->buffer.substr(position);

	return result;
}

Engine::Output::Buffer& Engine::Output::Buffer::operator<<(std::string& data)
{
	this->buffer + data + '\n';
}

Engine::Output::Buffer& Engine::Output::Buffer::operator<<(std::string_view data)
{
	this->buffer + data.data() + '\n';
}
