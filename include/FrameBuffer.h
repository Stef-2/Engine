#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "Window.h"

#include "glfw3.h"

#include "vector"
#include "iostream"

namespace Engine
{
	// OpenGL Frame Buffer Object wrapper
	// handles creation, initialization, activation and deactivation of frame buffer objects
	// provides access to textures to which the frame buffer can write
	class FrameBuffer
	{
	public:
		FrameBuffer();
		FrameBuffer(unsigned int width, unsigned int height);
		FrameBuffer(Engine::Window& window);
		~FrameBuffer();

		FrameBuffer(FrameBuffer&&) = default;
		FrameBuffer& operator= (FrameBuffer&&) = default;

		FrameBuffer(const FrameBuffer&) = default;
		FrameBuffer& operator= (const FrameBuffer&) = default;

		// possible FrameBuffer attachement types
		typedef enum class AttachmentType : GLenum
		{
			COLOR_ATTACHMENT,
			DEPTH_ATTACHMENT,
			STENCIL_ATTACHMENT
		};

		static FrameBuffer& GetCurrentFrameBuffer();
		unsigned int GetFBO() const;

		unsigned int GetColorBuffer() const;
		unsigned int GetColorBuffer(unsigned short index) const;
		unsigned int GetDepthBuffer() const;
		unsigned int GetStencilBuffer() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		void AddAttachment(AttachmentType type);
		void SetWidth(unsigned int width);
		void SetHeight(unsigned int height);

		void Activate() const;
		void Deactivate() const;

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