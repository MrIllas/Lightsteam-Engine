#include "SegmentHierarchy.h"

#include "ModuleScene.h"
#include "GameObject.h"

#include "MeshImporter.h"

SegmentHierarchy::SegmentHierarchy(bool enabled) : Segment(enabled)
{
	name = "Hierarchy";
}

SegmentHierarchy::~SegmentHierarchy()
{

}

void SegmentHierarchy::Start()
{
	sceneInstance = SceneProperties::Instance();
}

void SegmentHierarchy::Update()
{
	if (ImGui::Begin(name.c_str()))
	{
		DisplayGameObject(sceneInstance->root);
		RightClickMenu();
		
	}
	ImGui::End();

}

void SegmentHierarchy::DisplayGameObject(GameObject* go)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiDockNodeFlags_None;
	nodeFlags += ImGuiTreeNodeFlags_SpanAvailWidth;
	
	if (go->selected) nodeFlags += ImGuiTreeNodeFlags_Selected;
	//else nodeFlags += ImGuiTreeNodeFlags_None;

	if (!go->HasChildren()) nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	
	//Recursive Display
	const bool nodeOpen = ImGui::TreeNodeEx(go->name.c_str(), nodeFlags, go->name.c_str());
	if (nodeOpen)
	{
		if (ImGui::IsItemClicked()) { GetSelectedNode(go); }

		//if (ImGui::IsItemHovered()) hoveredGO = go;
		/*if (ImGui::IsItemClicked(1))
		{
			LOG(LOG_TYPE::NONE, "Clicked");*/
			RightClickMenu(go);
		//}

		for (int i = 0; i < go->children.size(); ++i)
		{
			DisplayGameObject(go->children[i]);
		}
		
		ImGui::TreePop();
	}
	
}

void SegmentHierarchy::RightClickMenu(GameObject* go)
{
	GameObject* auxGO = nullptr;
	if (go == nullptr) auxGO = sceneInstance->root;
	else auxGO = go;

	bool pop = ImGui::BeginPopupContextWindow();
	
	if (pop)
	{
		if (ImGui::BeginMenu("Nodes"))
		{
			if (ImGui::MenuItem("Empty Node"))	auxGO->AddChildren(new GameObject("Empty Node", false));
			if (ImGui::MenuItem("Spatial Node")) auxGO->AddChildren(new GameObject("Spatial Node"));
			if (ImGui::MenuItem("Mesh Node")) auxGO->AddChildren(new GameObject());

			if (ImGui::BeginMenu("Primitives"))
			{
				if (ImGui::MenuItem("Cube Node")) 	MeshImporter::ImportMesh("../Output/Assets/Primitives/Cube.fbx");
				if (ImGui::MenuItem("Sphere Node")) 	MeshImporter::ImportMesh("../Output/Assets/Primitives/Sphere.fbx");
				if (ImGui::MenuItem("Plane Node")) MeshImporter::ImportMesh("../Output/Assets/Primitives/Plane.fbx");
				if (ImGui::MenuItem("Pyramid Node")) MeshImporter::ImportMesh("../Output/Assets/Primitives/Pyramid.fbx");
				if (ImGui::MenuItem("Cylinder Node")) MeshImporter::ImportMesh("../Output/Assets/Primitives/Cylinder.fbx");
				if (ImGui::MenuItem("Capsule Node")) MeshImporter::ImportMesh("../Output/Assets/Primitives/Capsule.fbx");
				ImGui::EndMenu();
			}
			
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("DELETE", 0,false, go == nullptr ? false : true))
		{
			LOG(LOG_TYPE::ATTENTION, "DELETE GAMEOBJECT %s", go->name.c_str());
		}
		ImGui::EndPopup();
	}
}

void SegmentHierarchy::GetSelectedNode(GameObject* go)
{
	cleanSelected = true;

	cleanSelected = CleanSelected(sceneInstance->root);
	go->selected = true;
}

bool SegmentHierarchy::CleanSelected(GameObject* go)
{
	if (!cleanSelected) return false;

	go->selected = false;
	if (go->HasChildren())
	{
		for (int i = 0; i < go->children.size(); ++i)
		{
			CleanSelected(go->children[i]);
		}
	}
	return false;
}