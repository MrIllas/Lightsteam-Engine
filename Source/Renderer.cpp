#include "Renderer.h"

#include "FrameBuffer.h"
#include "CompMeshRenderer.h"

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
	//meshes.emplace_back(new MeshRenderer());
}

void Renderer::CleanUp()
{
	RELEASE(frameBuffer);

	

	/*for (uint i = 0; i < meshes.size(); ++i)
	{
		 RELEASE(meshes.front());
		 meshes.pop();
	}
	meshes.empty();*/
}

void Renderer::Render()
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
	for (uint i = 0; i < meshes.size(); ++i)
	{
		meshes.front()->Render();
		meshes.pop();
	}
}

void Renderer::PostUpdate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
#pragma endregion Renderer Update Phaces

void Renderer::QueueMesh(CompMeshRenderer* mesh)
{
	meshes.emplace(mesh);
}