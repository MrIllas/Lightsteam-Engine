#include "CompMeshRenderer.h"

#include "ModuleRenderer3D.h"
#include "Renderer.h"

CompMeshRenderer::CompMeshRenderer(GameObject* owner) : Component(owner)
{
	this->type = CO_TYPE::MESH_RENDERER;

	renInstance = RenderProperties::Instance();
}

CompMeshRenderer::~CompMeshRenderer()
{
	if (mesh != nullptr)
	{
		RELEASE(mesh);
	}
}

void CompMeshRenderer::Update()
{
	if (mesh == nullptr) return;

	renInstance->render->QueueMesh(this);
}

void CompMeshRenderer::Render()
{
	mesh->Draw();
}

MeshRenderer* CompMeshRenderer::GetMesh()
{
	return this->mesh;
}

void CompMeshRenderer::SetMesh(MeshRenderer* mesh)
{
	this->mesh = mesh;
}