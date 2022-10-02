#include "Renderer.h"


Renderer::Renderer(float2 size)
{
	frameBuffer = new FrameBuffer();
	Resize(size);
}

Renderer::~Renderer()
{

}

void Renderer::Init()
{

}

void Renderer::Start()
{
	
}

void Renderer::CleanUp()
{
	RELEASE(frameBuffer);
}

void Renderer::Draw()
{
	PreUpdate();
	Update();
	PostUpdate();
}

void Renderer::Resize(float2 size)
{
	this->size = size;
	frameBuffer->CreateBuffer(this->size.x, this->size.y);
}

#pragma region Renderer Update Phaces
void Renderer::PreUpdate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetFrameBuffer());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glLineWidth(2.0f);
}

void Renderer::Update()
{
	glBegin(GL_TRIANGLES);

	// front face =================
	//glVertex3f(1.0f, 1.0f, 1.0f); //0    
	//glVertex3f(-1.0, 1.0, 1.0); //1
	//glVertex3f(-1.0, -1.0, 1.0); //2

	//glVertex3f(1.0, -1.0, 1.0);  //3  
	//glVertex3f(1.0, -1.0, -1.0); //4
	//glVertex3f(1.0, 1.0, -1.0); //5
	//glVertex3f(-1.0, 1.0, -1.0); //6
	//glVertex3f(-1.0, -1.0, -1.0); //7

	// front face =================
	glColor3f(1.0f, 0.0f, 0.0f);  // Red
	glVertex3f(1.0f, 1.0f, 1.0f);    // v0-v1-v2
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);   // v2-v3-v0
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// right face =================
	glColor3f(1.0f, 0.0f, 1.0f);  // Violet
	glVertex3f(1.0f, 1.0f, 1.0f);    // v0-v3-v4
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(1.0, -1.0, -1.0);    // v4-v5-v0
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// top face ===================
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, 1.0f, 1.0f);    // v0-v5-v6
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);    // v6-v1-v0
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// back face =================
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow
	glVertex3f(-1.0, 1.0, -1.0); // 6-5-7
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0); //7-4-5
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(1.0, 1.0, -1.0);

	// left face =================
	glColor3f(0.0f, 0.0f, 1.0f);  // Blue
	glVertex3f(-1.0, 1.0, 1.0); //1-6-7
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0); //7-2-1
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);

	// Bottom face =================
	glColor3f(1.0f, 0.5f, 0.0f); // Orange
	glVertex3f(-1.0, -1.0, -1.0); //7-2-3
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0); //3-4-7
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);

	glEnd();
}

void Renderer::PostUpdate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
#pragma endregion Renderer Update Phaces