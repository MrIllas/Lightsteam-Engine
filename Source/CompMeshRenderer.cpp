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

#include "MeshImporter.h"

CompMeshRenderer::CompMeshRenderer(GameObject* owner, std::string uuid) : Component(owner, uuid)
{
	this->type = CO_TYPE::MESH_RENDERER;

	camInstance = CameraProperties::Instance();

	normals = Debug_Normals::OFF;
	SetNormalsString();
	normalsMagnitude = 0.25f;
}

CompMeshRenderer::~CompMeshRenderer()
{
	/*if (mesh != nullptr)
	{
		RELEASE(mesh);
	}*/
}

void CompMeshRenderer::Update()
{
	if (mesh == nullptr) return;

	//Update AABB
	float4x4 aux = owner->GetComponent<CompTransform>(TRANSFORM)->GetWorldMatrix();
	//Generate global OBB
	obb = this->mesh->mesh.localAABB;
	obb.Transform(aux.Transposed());

	//Generate global AABB
	aabb.SetNegativeInfinity();
	aabb.Enclose(obb);

	camInstance->editorCamera.renderer->QueueMesh(this);

	if (camInstance->gameCameras.size() != 0)
	{
		if (camInstance->gameCameras.at(camInstance->mainCameraId)->camera.renderer != nullptr)
			camInstance->gameCameras.at(camInstance->mainCameraId)->camera.renderer->QueueMesh(this);
	}
	
	
}

void CompMeshRenderer::UpdateGUI()
{
	ImGui::NewLine();

	if (ImGui::BeginCombo("Normals", normalsString.c_str()))
	{
		if (ImGui::Selectable("NONE")) normals = Debug_Normals::OFF;
		if (ImGui::Selectable("VERTEX")) normals = Debug_Normals::VERTEX;
		if (ImGui::Selectable("FACE")) normals = Debug_Normals::FACE;
		if (ImGui::Selectable("BOTH")) normals = Debug_Normals::BOTH;
		SetNormalsString();

		ImGui::EndCombo();
	}
		
	ImGui::NewLine();

	if (mesh == nullptr)
	{
		ImGui::Text("No mesh loaded!");
	}
	else
	{
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

void CompMeshRenderer::Render(Shader* shader, Shader* debugShader, Camera* camera, bool game)
{
	if (!active) return;

	if (owner->GetComponent<CompTexture>(MATERIAL) != nullptr && owner->GetComponent<CompTransform>(TRANSFORM) != nullptr)
	{
		if (game)
			mesh->LiteDraw(shader,
				camera,
				owner->GetComponent<CompTexture>(MATERIAL)->GetTexture(),
				owner->GetComponent<CompTransform>(TRANSFORM)->GetWorldMatrix());
		else
			mesh->FullDraw(shader,
				debugShader,
				camera,
				owner->GetComponent<CompTexture>(MATERIAL)->GetTexture(),
				owner->GetComponent<CompTransform>(TRANSFORM)->GetWorldMatrix(), normals);
	}
}

MeshRenderer* CompMeshRenderer::GetMesh()
{
	return this->mesh;
}

void CompMeshRenderer::SetMesh(MeshRenderer* mesh)
{
	this->mesh = mesh;
}

#pragma region Save/Load
nlohmann::ordered_json CompMeshRenderer::SaveUnique(nlohmann::JsonData data)
{
	if (mesh != nullptr)
	{
		data.SetString("Path", mesh->mesh.path);
	}
	
	data.SetInt("Normals", (int) normals);
	data.SetFloat("Normals_Magnitude", normalsMagnitude);

	return data.data;
}

void CompMeshRenderer::LoadUnique(nlohmann::JsonData data)
{
	std::string meshToLoad(data.GetString("Path"));
	mesh = new MeshRenderer(MeshImporter::LoadMesh(meshToLoad));

	normals = (Debug_Normals) data.GetInt("Normals");
	SetNormalsString();
	normalsMagnitude = data.GetFloat("Normals_Magnitude");
}

#pragma endregion Save & Load

void CompMeshRenderer::SetNormalsString()
{
	switch (normals)
	{
		case Debug_Normals::OFF: normalsString = "NONE"; break;
		case Debug_Normals::VERTEX: normalsString = "VERTEX"; break;
		case Debug_Normals::FACE: normalsString = "FACE"; break;
		case Debug_Normals::BOTH: normalsString = "BOTH"; break;
	}
}