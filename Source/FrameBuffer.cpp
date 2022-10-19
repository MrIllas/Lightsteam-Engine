#include "FrameBuffer.h"


FrameBuffer::FrameBuffer()
{
	frameBuffer = 0;
	textureColorbuffer = 0;
	renderBuffer = 0;
}

FrameBuffer::~FrameBuffer()
{

}

void FrameBuffer::CreateBuffer(int width, int height)
{
	CleanBuffer();
	
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//Attachments
	TextureAttachment(width, height);
	
	//DepthAndStencilAttachment(width, height);
	
	//Since renderbuffer objects are write-only they are often used as depth and stencil attachments, 
	//since most of the time we don't really need to read values from them, but we do care about depth 
	//and stencil testing.
	RenderBufferAttachment(width, height);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		//LOG("SUCCESS: Framebuffer is complete!");
	}
	else
	{
		LOG(LOG_TYPE::ERRO, "ERROR: Framebuffer is not complete!")
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::CleanBuffer()
{
	if(frameBuffer != 0) glDeleteFramebuffers(1, &frameBuffer);
	if(textureColorbuffer != 0) glDeleteTextures(1, &textureColorbuffer);
	if(renderBuffer != 0) glDeleteRenderbuffers(1, &renderBuffer);
	
}

#pragma region Attachments

void FrameBuffer::TextureAttachment(int width, int height)
{
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
}

void FrameBuffer::DepthAndStencilAttachment(int width, int height)
{
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
	);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAndStencilBuffer, 0);
}

void FrameBuffer::RenderBufferAttachment(int width, int height)
{
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
}

#pragma endregion Attachments