#include "CompMeshRenderer.h"

#include "ModuleCamera3D.h"
#include "Renderer.h"
#include "Shader.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

CompMeshRenderer::CompMeshRenderer(GameObject* owner) : Component(owner)
{
	this->type = CO_TYPE::MESH_RENDERER;

	camInstance = CameraProperties::Instance();
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

	camInstance->editorCamera.renderer->QueueMesh(this);
}

void CompMeshRenderer::UpdateGUI()
{
	if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_Leaf))
	{
		ImGui::Checkbox("Active##MeshRenderer", &this->active);

		/*ImGui::Button
		ImGui::InputText("", mesh->texture.path.c_str(), IM_ARRAYSIZE(mesh->texture.path.c_str()));*/
		std::string txtPath = "Texture Path: " + mesh->texture.path;
		ImGui::Text(txtPath.c_str());
		ImGui::Button("Diffuse Texture", { 300, 30 });
		DragAndDrop();
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

void CompMeshRenderer::DragAndDrop()
{
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* drop = ImGui::AcceptDragDropPayload("texture");
		if (drop)
		{
			std::string* aux = (std::string*)drop->Data;
			mesh->texture.path = *aux;

			LOG(LOG_TYPE::NONE, "Drop-> %s", aux)
		}
	}
}