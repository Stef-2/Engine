#include "OpenGLObject.h"

Engine::OpenGLObject* Engine::OpenGLObject::currentObject = nullptr;

unsigned int Engine::OpenGLObject::GetHandle()
{
	return this->handle;
}

Engine::OpenGLObject::OpenGLObject()
{
	this->handle = 0u;
}

Engine::Buffer::Buffer(BufferType type, BufferUsage usage = Engine::Buffer::BufferUsage::DYNAMIC_DRAW)
	: bufferType(type), bufferUsage(usage)
{
	this->bufferSize = 0u;
	this->mapped = false;
	this->Generate();
}

Engine::Buffer::~Buffer()
{
	if (this->handle)
		glDeleteBuffers(1, &this->handle);
}

Engine::Buffer::BufferType Engine::Buffer::GetBufferType() const
{
	return bufferType;
}

Engine::Buffer::BufferUsage Engine::Buffer::GetBufferUsage() const
{
	return bufferUsage;
}

void Engine::Buffer::SetBufferUsage(BufferUsage value)
{
	bufferUsage = value;
}

unsigned int Engine::Buffer::GetBufferSize() const
{
	return bufferSize;
}

void Engine::Buffer::SetData(const void* data, unsigned int size)
{
	if (this->handle) {
		glNamedBufferData(this->handle, size, data, static_cast<GLenum>(GetBufferType()));
		this->bufferSize = size;
	}
}

void Engine::Buffer::SetDataRange(const void* data, unsigned int offset, unsigned int size)
{
	glNamedBufferSubData(this->handle, offset, size, data);
}

void Engine::Buffer::CopyDataTo(Buffer& other, unsigned int readOffset, unsigned int writeOffset, unsigned int size)
{
	glCopyNamedBufferSubData(this->handle, other.GetHandle(), readOffset, writeOffset, size);
}

void Engine::Buffer::Clear(GLenum format, GLenum internalFormat)
{
	glClearNamedBufferData(this->handle, internalFormat, format, static_cast<GLenum>(GetBufferType()), nullptr);
}

void Engine::Buffer::BindBase(unsigned int index, unsigned int location)
{
	glBindBufferBase(static_cast<GLenum>(this->bufferType), index, location);
}

void Engine::Buffer::Invalidate()
{
	glInvalidateBufferData(this->handle);
}

void Engine::Buffer::WriteBufferData(void* data)
{
	glGetNamedBufferSubData(this->handle, 0, this->GetBufferSize(), data);
}

void* Engine::Buffer::MapBuffer(BufferMappingUsage usageBits)
{
	void* map = glMapNamedBuffer(this->handle, static_cast<GLenum>(usageBits));
	
	if (map) {
		this->mapped = true;
		return map;
	}

	return nullptr;
}

void Engine::Buffer::UnmapBuffer()
{
	if (this->mapped) {
		glUnmapNamedBuffer(this->handle);
		this->mapped = false;
	}
}

void Engine::Buffer::CopyDataTo(Buffer& other)
{
	glCopyNamedBufferSubData(this->handle, other.handle, 0, 0, this->GetBufferSize());
}

void Engine::Buffer::Activate()
{
	glBindBuffer(static_cast<GLenum>(GetBufferType()), this->handle);
	OpenGLObject::currentObject = this;
}

void Engine::Buffer::Generate()
{
	glGenBuffers(1, &this->handle);
}