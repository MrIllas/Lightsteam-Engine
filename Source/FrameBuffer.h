#pragma once

#include "Globals.h"
#include "Glew/include/glew.h"


//For now it's a singleton for early testing purpouses, will change later on development.
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void CreateBuffer(int width = 1280, int height = 720);

	//GETTERS
	int GetFrameBuffer() { return frameBuffer; }
	int GetTextureBuffer() { return textureColorbuffer; }
	int GetDepthAndStencilBuffer() { return depthAndStencilBuffer; }
	int GetRenderBuffer() { return renderBuffer; }

private:
	void TextureAttachment(int width, int height);
	void DepthAndStencilAttachment(int width, int height);
	void RenderBufferAttachment(int width, int height);

	void CleanBuffer();

private:
	unsigned int frameBuffer = 0;
	unsigned int textureColorbuffer = 0;
	unsigned int depthAndStencilBuffer = 0;
	unsigned int renderBuffer = 0;
};

