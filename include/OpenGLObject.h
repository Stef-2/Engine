#pragma once

#ifndef __glad_h_
#include "glad/glad.h"
#define __glad_h_
#endif

#include "glfw3.h"
#include "Shared.h"

#include "vector"
#include "any"

namespace Engine
{
	class OpenGLObject
	{
	public:
		OpenGLObject();

		unsigned int GetHandle();
	protected:
		unsigned int handle;

		static OpenGLObject* currentObject;
	};

	// ===========================================================================

	class Buffer : public OpenGLObject
	{
	public:
		// different types of OpenGL buffers
		typedef enum class BufferType : GLenum
		{
			ARRAY_BUFFER = GL_ARRAY_BUFFER,
			ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
			COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
			COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
			PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
			PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
			QUERY_BUFFER = GL_QUERY_BUFFER,
			TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
			TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
			UNIFORM_BUFFER = GL_UNIFORM_BUFFER,
			DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
			ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
			DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
			SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER
		};

		// hints on ways buffers will be used, used by OpenGL for optimization
		typedef enum class BufferUsage : GLenum
		{
			STATIC_READ = GL_STATIC_READ,
			STATIC_DRAW = GL_STATIC_DRAW,
			STATIC_COPY = GL_STATIC_COPY,

			DYNAMIC_READ = GL_DYNAMIC_READ,
			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
			DYNAMIC_COPY = GL_DYNAMIC_COPY,

			STREAM_READ = GL_STREAM_READ,
			STREAM_DRAW = GL_STREAM_DRAW,
			STREAM_COPY = GL_STREAM_COPY
		};

		// defines the way in which mapping is performed in case of mapped buffers
		typedef enum class BufferMappingUsage : GLbitfield
		{
			READ_BIT = GL_MAP_READ_BIT,
			WRITE_BIT = GL_MAP_WRITE_BIT,
			PERSISTENT_BIT = GL_MAP_PERSISTENT_BIT,
			COHERENT_BIT = GL_MAP_COHERENT_BIT,

			// optional
			INVALIDATE_RANGE_BIT = GL_MAP_INVALIDATE_RANGE_BIT,
			INVALIDATE_BUFFER_BIT = GL_MAP_INVALIDATE_BUFFER_BIT,
			FLUSH_EXPLICIT_BIT = GL_MAP_FLUSH_EXPLICIT_BIT,
			UNSYNCHRONIZED_BIT = GL_MAP_UNSYNCHRONIZED_BIT
		};

		Buffer(BufferType, BufferUsage);

		Buffer(Buffer&&) = delete;
		Buffer& operator= (Buffer&&) = delete;

		Buffer(const Buffer&) = delete;
		Buffer& operator= (const Buffer&) = delete;

		~Buffer();

		// bind this buffer
		void Activate();

		// write data to buffer
		void SetData(const void* data, unsigned int size);
		void SetDataRange(const void* data, unsigned int offset, unsigned int size);

		// pass data to another buffer
		void CopyDataTo(Buffer& other);
		void CopyDataTo(Buffer& other, unsigned int readOffset, unsigned int writeOffset, unsigned int size);
		
		// clear buffer contents
		void Clear(GLenum format, GLenum internalFormat);
		// bind the buffer to currently bound shaders binding spot index, at shader location
		void BindBase(unsigned int index, unsigned int location);
		void Invalidate();

		// write buffer contents into provided location
		void WriteBufferData(void* location);

		// map buffer to a client location, provides a pointer through which buffer can be accessed
		void* MapBuffer(BufferMappingUsage usageBits);
		// clear mapping
		void UnmapBuffer();
		
		Engine::Buffer::BufferType GetBufferType() const;
		Engine::Buffer::BufferUsage GetBufferUsage() const;
		void SetBufferUsage(Engine::Buffer::BufferUsage value);
		unsigned int GetBufferSize() const;

	protected:
		void Generate();
		unsigned int bufferSize;

		BufferType bufferType;
		BufferUsage bufferUsage;

		bool mapped;
	};
}

