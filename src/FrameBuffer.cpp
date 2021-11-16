#include "FrameBuffer.h"

Engine::FrameBuffer* Engine::FrameBuffer::currentFramebuffer = {};

Engine::FrameBuffer::FrameBuffer()
{
	this->width = {Engine::Motor::GetInstance().GetWindow().GetDimensions().x};
	this->height = { Engine::Motor::GetInstance().GetWindow().GetDimensions().y};
	this->FBO = {};

	this->colorBuffer = {};
	this->depthBuffer = {};
	this->stencilBuffer = {};
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

unsigned int Engine::FrameBuffer::GetFBO()
{
	return this->FBO;
}

unsigned int Engine::FrameBuffer::GetColorBuffer()
{
	if (this->colorBuffer.size())
		return this->colorBuffer.at(0);
	else
		std::cerr << "frame buffer has no color attachments" << std::endl;
}

unsigned int Engine::FrameBuffer::GetColorBuffer(unsigned short index)
{
	if (this->colorBuffer.size() > index)
		return this->colorBuffer.at(index);
	else
		std::cerr << "invalid index: " << index << std::endl;
}

unsigned int Engine::FrameBuffer::GetDepthBuffer()
{
	if (this->depthBuffer)
		return this->depthBuffer;
	else
		std::cerr << "frame buffer has no depth attachments" << std::endl;
}

unsigned int Engine::FrameBuffer::GetStencilBuffer()
{
	if (this->stencilBuffer)
		return this->stencilBuffer;
	else
		std::cerr << "frame buffer has no stencil attachments" << std::endl;
}

void Engine::FrameBuffer::AddAttachment(AttachmentType type)
{
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
		glBindRenderbuffer(GL_FRAMEBUFFER, this->FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + this->colorBuffer.size() - 1, GL_TEXTURE_2D, this->colorBuffer.back(), 0);
		glBindRenderbuffer(GL_FRAMEBUFFER, 0u);

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
		glBindRenderbuffer(GL_FRAMEBUFFER, this->FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthBuffer, 0);
		glBindRenderbuffer(GL_FRAMEBUFFER, 0u);

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
		glBindRenderbuffer(GL_FRAMEBUFFER, this->FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->stencilBuffer, 0);
		glBindRenderbuffer(GL_FRAMEBUFFER, 0u);

		break;
	}

	default:
		break;
	}
}

void Engine::FrameBuffer::Activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	Engine::FrameBuffer::currentFramebuffer = this;
}

void Engine::FrameBuffer::Deactivate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0u);
	Engine::FrameBuffer::currentFramebuffer = nullptr;
}

void Engine::FrameBuffer::Setup()
{
	glGenFramebuffers(1, &this->FBO);
	
}
