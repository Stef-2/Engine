#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "Engine.h"

#include "glfw3.h"

#include "vector"
#include "iostream"

namespace Engine
{

	// OpenGL Frame buffer object wrapper
	// handles creation, initialization, activation and deactivation of frame buffer objects
	// provides access to textures to which the frame buffer can write
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		FrameBuffer(FrameBuffer&&) = default;
		FrameBuffer& operator= (FrameBuffer&&) = default;

		FrameBuffer(const FrameBuffer&) = default;
		FrameBuffer& operator= (const FrameBuffer&) = default;

		typedef enum class AttachmentType : GLenum
		{
			COLOR_ATTACHMENT,
			DEPTH_ATTACHMENT,
			STENCIL_ATTACHMENT
		};

		static FrameBuffer& GetCurrentFrameBuffer();
		unsigned int GetFBO();

		unsigned int GetColorBuffer();
		unsigned int GetColorBuffer(unsigned short index);
		unsigned int GetDepthBuffer();
		unsigned int GetStencilBuffer();

		void AddAttachment(AttachmentType type);

		void Activate();
		void Deactivate();

	private:
		void Setup();

		unsigned int width;
		unsigned int height;
		unsigned int FBO;

		std::vector<unsigned int> colorBuffer;
		unsigned int depthBuffer;
		unsigned int stencilBuffer;

		static FrameBuffer* currentFramebuffer;

	};

}

#endif // FRAMEBUFFER_H