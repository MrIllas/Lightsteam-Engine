#include "CompMeshRenderer.h"

#include "ModuleRenderer3D.h"
#include "Renderer.h"
#include "Shader.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

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

void CompMeshRenderer::UpdateGUI()
{
	if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_Leaf))
	{
		ImGui::Checkbox("Active##MeshRenderer", &this->active);
	}
}

void CompMeshRenderer::Render(Shader* shader)
{
	if (!active) return;
	mesh->Draw(shader);
}

MeshRenderer* CompMeshRenderer::GetMesh()
{
	return this->mesh;
}

void CompMeshRenderer::SetMesh(MeshRenderer* mesh)
{
	this->mesh = mesh;
}