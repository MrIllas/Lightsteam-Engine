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
	void CleanBuffer();

	//GETTERS
	int GetFrameBuffer() { return frameBuffer; }
	int GetTextureBuffer() { return textureColorbuffer; }
	int GetDepthAndStencilBuffer() { return depthAndStencilBuffer; }
	int GetRenderBuffer() { return renderBuffer; }

	static FrameBuffer* Instance();
	static void Delete();

private:
	void TextureAttachment(int width, int height);
	void DepthAndStencilAttachment(int width, int height);
	void RenderBufferAttachment(int width, int height);

private:
	unsigned int frameBuffer;
	unsigned int textureColorbuffer;
	unsigned int depthAndStencilBuffer;
	unsigned int renderBuffer;

	static FrameBuffer* instance;
};

