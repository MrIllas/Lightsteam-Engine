#include "CompMeshRenderer.h"

#include "ModuleCamera3D.h"
#include "Renderer.h"
#include "Shader.h"

#include "GameObject.h"
#include "CompTexture.h"
#include "CompTransform.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

CompMeshRenderer::CompMeshRenderer(GameObject* owner) : Component(owner)
{
	this->type = CO_TYPE::MESH_RENDERER;

	camInstance = CameraProperties::Instance();

	displayNormals = false;
	normalsMagnitude = 0.25f;
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
		ImGui::SameLine();
		ImGui::Checkbox("Display Normals", &displayNormals);

		if (displayNormals)
		{
			ImGui::Checkbox("Face Normals ", &faceNormals);
			if (ImGui::SliderFloat("Line Magnitude", &normalsMagnitude, 0.1f, 1.0f, "%.2f"))
			{
				mesh->CleanNormals();
				mesh->CreateNormals(normalsMagnitude);
			}
		}
		

		//Vertex
		std::string txt = "Vertices: ";
		txt += std::to_string(mesh->mesh.vertices.size());
		ImGui::Text(txt.c_str());

		//Indices
		txt = "Indices: ";
		txt += std::to_string(mesh->mesh.indices.size());
		ImGui::Text(txt.c_str());

		//Faces
		txt = "Faces: ";
		txt += std::to_string(mesh->mesh.numFaces);
		ImGui::Text(txt.c_str());
	}
}

void CompMeshRenderer::Render(Shader* shader, Shader* debugShader)
{
	if (!active) return;

	mesh->Draw(shader,
		owner->GetComponent<CompTexture>(MATERIAL)->GetTexture(),
		owner->GetComponent<CompTransform>(TRANSFORM)->GetWorldMatrix());

	if (displayNormals) 
		mesh->DrawNormals(
			debugShader,
			owner->GetComponent<CompTransform>(TRANSFORM)->GetWorldMatrix(),
		faceNormals);
}

MeshRenderer* CompMeshRenderer::GetMesh()
{
	return this->mesh;
}

void CompMeshRenderer::SetMesh(MeshRenderer* mesh)
{
	this->mesh = mesh;
}