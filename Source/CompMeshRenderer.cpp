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

#include "ModuleResources.h"
#include "ResourceModel.h"

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
	if (mesh != nullptr)
	{
		if (!mesh->uuid.empty() && !mesh->modelUuid.empty())
		{
			ResourceModel* resource = nullptr;

			//Needs to check if contains the resource since Resources are created after imporing the Model to the library.
			if(ResourceProperties::Instance()->resources.count(mesh->modelUuid))
				resource = (ResourceModel*) ResourceProperties::Instance()->resources[mesh->modelUuid];
		
			if(resource != nullptr) resource->meshRendererMap->at(mesh->uuid)->DecreaseRC();
		}
	}
}

void CompMeshRenderer::Update()
{
	if (mesh == nullptr ) return;
	if (mesh->planDelete)
	{
		mesh = nullptr;
		return;
	}
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

	MeshDrop();

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

void CompMeshRenderer::MeshDrop()
{
	std::string buttonValue = "Drop mesh";
	ImGui::Button(buttonValue.c_str(), {100, 100});

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MeshCFF"))
		{
			IM_ASSERT(payload->DataSize == sizeof(std::string));
			std::string item = *static_cast<const std::string*>(payload->Data);

			std::string item2;

			size_t pos = item.find_last_of("/");
			if (pos != std::string::npos)
			{
				item2 = item.substr(pos + 1);
				item.erase(pos);
			}
				 
			
		

			ResourceModel* model = (ResourceModel*)ResourceProperties::Instance()->resources[item];

			if (mesh != nullptr)
			{
				ResourceModel* currentModel = (ResourceModel*)ResourceProperties::Instance()->resources[mesh->modelUuid];
				currentModel->meshRendererMap->at(mesh->uuid)->DecreaseRC();
			}
				
			mesh = MeshImporter::ImportMeshFromLibrary(model, item2);
		}
	}

}

void CompMeshRenderer::Render(Shader* shader, Shader* debugShader, Camera* camera, bool game)
{
	if (!active || mesh == NULL) return;

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
		data.SetString("Mesh Uuid", mesh->uuid);
		data.SetString("Model Uuid", mesh->modelUuid);
	}
	
	data.SetInt("Normals", (int) normals);
	data.SetFloat("Normals_Magnitude", normalsMagnitude);

	return data.data;
}

void CompMeshRenderer::LoadUnique(nlohmann::JsonData data)
{
	std::string meshToLoad(data.GetString("Mesh Uuid"));
	std::string modelToLoad(data.GetString("Model Uuid"));

	ResourceModel* model = (ResourceModel*)ResourceProperties::Instance()->resources[modelToLoad];
	mesh = MeshImporter::ImportMeshFromLibrary(model, meshToLoad);

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