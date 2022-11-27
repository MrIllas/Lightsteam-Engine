#include "Renderer.h"

#include "GameObject.h"
#include "CompMeshRenderer.h"
#include "Shader.h"

#include "FrameBuffer.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"

Renderer::Renderer(float2 size, Camera* camera)
{
	owner = camera;
	frameBuffer = new FrameBuffer();
	defaultFrameBuffer = new FrameBuffer();
	Resize(size);

	debugShader = new Shader(DEBUG_VERTEX_SHADER, DEBUG_FRAGMENT_SHADER, "DEBUG SHADER");
	baseShader = new Shader(BASE_VERTEX_SHADER, BASE_FRAGMENT_SHADER, "BASE SHADER");
	screenShader = new Shader("Shader/Post-processing/pp_Normal.vs", "Shader/Post-processing/pp_Normal.fs", "DEPHT SHADER");
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

void Renderer::Render(bool game)
{
	PreUpdate();
	Update(game);
	PostUpdate();
}

void Renderer::Resize(float2 size)
{
	this->size = size;
	frameBuffer->CreateBuffer(this->size.x, this->size.y);
	defaultFrameBuffer->CreateBuffer(this->size.x, this->size.y);
}

#pragma region Renderer Update Phases
void Renderer::PreUpdate()
{
	//First Pass for Post-processing
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetFrameBuffer());
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFrameBuffer->GetFrameBuffer());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	//glEnable(GL_DEPTH_TEST);
	glLineWidth(1.0f);
	
	//glEnableClientState(GL_VERTEX_ARRAY); //Type of data
	//glVertexPointer(3, GL_FLOAT, 0, NULL); //Use bind buffer as vertices

}

void Renderer::Update(bool game)
{
	while (meshes.size())
	{
		if (meshes.front() != nullptr)
		{
			meshes.front()->Render(baseShader, debugShader, owner, game);
		}
		meshes.pop();
	}
	numOfMeshes = 0;


	//Debug Meshes
	while (debugMeshes.size())
	{
		if (debugMeshes.front() != nullptr)
		{
			debugMeshes.front()->mesh->DrawFrustumBox(debugShader, owner, debugMeshes.front()->model);
		}
		debugMeshes.pop();
	}

}

void Renderer::PostUpdate()
{
	//Second Pass for Post-processing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::PostProcessing()
{
	screenShader->Use();
	screenShader->SetInt("screenTexture", frameBuffer->GetTextureBuffer());
}

#pragma endregion Renderer Update Phases

void Renderer::QueueMesh(CompMeshRenderer* mesh)
{
	if (owner->ContainsBBox(mesh->GetAABB())) //Checks if mesh is inside the render's camera frustum
	{
		meshes.emplace(mesh);
		numOfMeshes++;
	}
	//else LOG(LOG_TYPE::ATTENTION, "The mesh of the GO '%s' is outside frustum!", mesh->owner->name.c_str());
}

void Renderer::QueueDebug(DebugMesh* mesh)
{
	debugMeshes.emplace(mesh);
}