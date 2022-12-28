#include "SegmentInspector.h"

#include "ModuleScene.h"
#include "GameObject.h"

#include "ImGui/misc/cpp/imgui_stdlib.h"

SegmentInspector::SegmentInspector(bool enabled) : Segment(enabled)
{
	name = "Inspector";
}

SegmentInspector::~SegmentInspector()
{

}

void SegmentInspector::Start()
{
	sceneInstance = SceneProperties::Instance();
}

void SegmentInspector::Update()
{
	if (ImGui::Begin(name.c_str()))
	{
		//Each Component manages its own ImGui
		GameObject* go = IterateGameObject(sceneInstance->root);

		if (go != nullptr)
		{
			float segWidth = ImGui::GetWindowWidth(); //Segment Inspector width.
			
			if (ImGui::InputText("##Name", &go->name))
			{
				if (go->name.size() == 0) go->name = "NoName";
			}
			ImGui::Text("UUID: ");
			ImGui::SameLine();
			ImGui::Text(go->uuid.c_str());

			if (go != sceneInstance->root)
			{
				for (auto const& comp : go->components)
				{
					std::string aux = CompTypeToString(comp.second->type);

					if (ImGui::CollapsingHeader(aux.c_str(), ImGuiTreeNodeFlags_Leaf))
					{
						//Active checkbox
						aux.insert(0, "Active##");
						ImGui::Checkbox(aux.c_str(), &comp.second->active);

						//Delete component button
						aux.erase(0, 8);
						aux.insert(0, "DELETE##");
						ImGui::SameLine(segWidth - 60);
						if (ImGui::Button(aux.c_str(), ImVec2(0, 0)))
						{
							go->DeleteComponent(comp.second->type);
							break;
						}
						ImGui::Separator();


						comp.second->UpdateGUI();
						ImGui::NewLine();
					}
				}
				go->UpdateCompMenuGUI();
			}		
		}
		
	}
	ImGui::End();
}

GameObject* SegmentInspector::IterateGameObject(GameObject* go)
{
	GameObject* toReturn = go;

	if (go->selected) return go;
	else
	{
		for (int i = 0; i < go->children.size(); ++i)
		{
			toReturn = IterateGameObject(go->children[i]);
			
			if ( toReturn  != nullptr && toReturn->selected) return toReturn;

			if (go->children[i]->selected)
			{
				return go->children[i];
			}
		}
	}

	return nullptr;
}