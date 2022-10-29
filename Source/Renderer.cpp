#include "Renderer.h"

#include "FrameBuffer.h"
#include "CompMeshRenderer.h"
#include "Shader.h"

#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"

Renderer::Renderer(float2 size)
{
	frameBuffer = new FrameBuffer();
	Resize(size);

	debugShader = new Shader(DEBUG_VERTEX_SHADER, DEBUG_FRAGMENT_SHADER);
	baseShader = new Shader(BASE_VERTEX_SHADER, BASE_FRAGMENT_SHADER);
}

Renderer::~Renderer()
{
	if (frameBuffer != nullptr) RELEASE(frameBuffer);

	if (baseShader != nullptr) RELEASE(baseShader);

	if (debugShader != nullptr) RELEASE(debugShader);
}

void Renderer::Init()
{

}

void Renderer::Start()
{

}

void Renderer::CleanUp()
{
	
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
	
	//glEnableClientState(GL_VERTEX_ARRAY); //Type of data
	//glVertexPointer(3, GL_FLOAT, 0, NULL); //Use bind buffer as vertices

}

void Renderer::Update()
{
	while (meshes.size())
	{
		meshes.front()->Render(baseShader, debugShader);
		meshes.pop();
	}
}

void Renderer::PostUpdate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisableClientState(GL_VERTEX_ARRAY);
}
#pragma endregion Renderer Update Phaces

void Renderer::QueueMesh(CompMeshRenderer* mesh)
{
	meshes.emplace(mesh);
}