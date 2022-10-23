#include "SegmentInspector.h"

#include "ModuleScene.h"
#include "GameObject.h"

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
			for (auto const& comp : go->components)
			{
				comp.second->UpdateGUI();
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