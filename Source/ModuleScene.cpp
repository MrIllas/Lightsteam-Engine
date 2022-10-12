#include "ModuleScene.h"

#include "GameObject.h"


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{

}

bool ModuleScene::Init()
{
	root = new GameObject();
	root->DeleteComponent(TRANSFORM);

	return true;
}

bool ModuleScene::Start()
{


	return true;
}

bool ModuleScene::CleanUp()
{
	RELEASE(root);

	return true;
}

UpdateStatus ModuleScene::PreUpdate()
{


	return UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::Update()
{

	//Update Game Objects
	UpdateGameObject(root);


	return UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::PostUpdate()
{


	return UPDATE_CONTINUE;
}


void ModuleScene::UpdateGameObject(GameObject* go)
{
	go->Update();

	if (go->HasChildren())
	{
		for (int i = 0; i < go->children.size(); ++i)
		{
			UpdateGameObject(go->children[i]);
		}
	}
}

#pragma region Save/Load Settings
void ModuleScene::LoadSettingsData(pugi::xml_node& load)
{

}

void ModuleScene::SaveSettingsData(pugi::xml_node& save)
{

}
#pragma endregion Save & Load of Settings