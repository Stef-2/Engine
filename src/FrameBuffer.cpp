#include "FrameBuffer.h"

Engine::FrameBuffer* Engine::FrameBuffer::currentFramebuffer = {};

Engine::FrameBuffer::FrameBuffer()
{
	this->width = {};
	this->height = {};
	this->FBO = {};

	this->colorBuffer = {};
	this->depthBuffer = {};
	this->stencilBuffer = {};

	this->Setup();
}

Engine::FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->FBO = {};

	this->colorBuffer = {};
	this->depthBuffer = {};
	this->stencilBuffer = {};

	this->Setup();
}

Engine::FrameBuffer::FrameBuffer(Engine::Window& window)
{
	this->width = window.GetDimensions().x;
	this->height = window.GetDimensions().y;
	this->FBO = {};

	this->colorBuffer = {};
	this->depthBuffer = {};
	this->stencilBuffer = {};

	this->Setup();
}

Engine::FrameBuffer::~FrameBuffer()
{
	// clean up bound textures
	for (size_t i = 0; i < this->colorBuffer.size(); i++)
		glDeleteTextures(1, &this->colorBuffer.at(i));

	if (this->depthBuffer)
		glDeleteTextures(1, &this->depthBuffer);

	if (this->stencilBuffer)
		glDeleteTextures(1, &this->stencilBuffer);

	// delete the frame buffer itself
	glDeleteFramebuffers(1, &this->FBO);

	// reset the current one if it was pointing to us
	if (Engine::FrameBuffer::currentFramebuffer == this)
		Engine::FrameBuffer::currentFramebuffer == nullptr;
}

Engine::FrameBuffer& Engine::FrameBuffer::GetCurrentFrameBuffer()
{
	return *Engine::FrameBuffer::currentFramebuffer;
}

unsigned int Engine::FrameBuffer::GetFBO() const
{
	return this->FBO;
}

unsigned int Engine::FrameBuffer::GetColorBuffer() const
{
	if (this->colorBuffer.size())
		return this->colorBuffer.at(0);
	else
		std::cerr << "frame buffer has no color attachments" << std::endl;
}

unsigned int Engine::FrameBuffer::GetColorBuffer(unsigned short index) const
{
	if (this->colorBuffer.size() > index)
		return this->colorBuffer.at(index);
	else
		std::cerr << "invalid index: " << index << std::endl;
}

unsigned int Engine::FrameBuffer::GetDepthBuffer() const
{
	if (this->depthBuffer)
		return this->depthBuffer;
	else
		std::cerr << "frame buffer has no depth attachments" << std::endl;
}

unsigned int Engine::FrameBuffer::GetStencilBuffer() const
{
	if (this->stencilBuffer)
		return this->stencilBuffer;
	else
		std::cerr << "frame buffer has no stencil attachments" << std::endl;
}

unsigned int Engine::FrameBuffer::GetWidth() const
{
	return this->width;
}

unsigned int Engine::FrameBuffer::GetHeight() const
{
	return this->height;
}

void Engine::FrameBuffer::AddAttachment(AttachmentType type)
{
	// check if the frame buffer object has been generated, if not, make one
	if (!this->FBO)
		this->Setup();

	switch (type)
	{
	case Engine::FrameBuffer::AttachmentType::COLOR_ATTACHMENT:
	{
		// add a new texture to the stack and set it up
		this->colorBuffer.push_back({});
		glGenTextures(1, &this->colorBuffer.back());

		glBindTexture(GL_TEXTURE_2D, this->colorBuffer.back());

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// attach it to the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + this->colorBuffer.size() - 1, GL_TEXTURE_2D, this->colorBuffer.back(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0u);

		break; 
	}

	case Engine::FrameBuffer::AttachmentType::DEPTH_ATTACHMENT:
	{
		// add a new texture to the stack and set it up
		glGenTextures(1, &this->depthBuffer);

		glBindTexture(GL_TEXTURE_2D, this->depthBuffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// attach it to the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthBuffer, 0);

		// in case we don't have a color buffer, we need to explicitly tell OpenGL we're not using it
		if (!this->colorBuffer.size()) {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0u);

		break;
	}
	case Engine::FrameBuffer::AttachmentType::STENCIL_ATTACHMENT:
	{
		// add a new texture to the stack and set it up
		glGenTextures(1, &this->stencilBuffer);

		glBindTexture(GL_TEXTURE_2D, this->stencilBuffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_COMPONENTS, this->width, this->height, 0, GL_STENCIL_COMPONENTS, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// attach it to the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->stencilBuffer, 0);

		// in case we don't have a color buffer, we need to explicitly tell OpenGL we're not using it
		if (!this->colorBuffer.size()) {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0u);

		break;
	}

	default:
		break;
	}
}

void Engine::FrameBuffer::SetWidth(unsigned int width)
{
	this->width = width;
}

void Engine::FrameBuffer::SetHeight(unsigned int height)
{
	this->height = height;
}

void Engine::FrameBuffer::Activate() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	Engine::FrameBuffer::currentFramebuffer = const_cast<Engine::FrameBuffer*>(this);
}

void Engine::FrameBuffer::Deactivate() const
{
	// reset back to default context framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0u);
	Engine::FrameBuffer::currentFramebuffer = nullptr;
}

void Engine::FrameBuffer::Setup()
{
	glGenFramebuffers(1, &this->FBO);
}
