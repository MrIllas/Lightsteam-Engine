#include "SegmentHierarchy.h"

#include "ModuleScene.h"
#include "GameObject.h"

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
	//else { go->selected = false; }
	//Get Node Flags
	ImGuiTreeNodeFlags nodeFlags = ImGuiDockNodeFlags_None;
	
	if (go->selected) nodeFlags += ImGuiTreeNodeFlags_Selected;
	//else nodeFlags += ImGuiTreeNodeFlags_None;

	if (!go->HasChildren()) nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	
	//Recursive Display
	const bool nodeOpen = ImGui::TreeNodeEx(go->name.c_str(), nodeFlags);
	if (nodeOpen)
	{
		if (ImGui::IsItemClicked()) { GetSelectedNode(go); }

		for (int i = 0; i < go->children.size(); ++i)
		{
			DisplayGameObject(go->children[i]);
		}
		
		ImGui::TreePop();
	}
}

void SegmentHierarchy::RightClickMenu()
{
	//
	
	//if (ImGui::BeginPopupContextWindow() || ImGui::BeginPopupContextItem())
	if(ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Add Node"))
		{

		}

		if (ImGui::MenuItem("DELETE"))
		{

		}
		ImGui::EndPopup();
	}
	
}

void SegmentHierarchy::GetSelectedNode(GameObject* go)
{
	go->selected = true;
	cleanSelected = true;
}