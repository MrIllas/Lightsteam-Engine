#include "ModuleScene.h"

#include "GameObject.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "CompMeshRenderer.h"

#pragma region SceneProperties
SceneProperties::SceneProperties()
{
	root = new GameObject("Scene");
	root->DeleteComponent(TRANSFORM);
}

SceneProperties* SceneProperties::Instance()
{
	if (instance == nullptr) instance = new SceneProperties();

	return instance;
}

GameObject* SceneProperties::GetSelectedGO(GameObject* go)
{
	GameObject* toReturn = go;
	if (go == nullptr) toReturn = root;

	if (toReturn->selected) return toReturn;
	else
	{
		GameObject* aux = nullptr;
		for (int i = 0; i < toReturn->children.size(); ++i)
		{
			aux = GetSelectedGO(toReturn->children[i]);
			if (aux != nullptr && aux->selected) return aux;
		}
	}

	return nullptr;
}

void SceneProperties::Delete()
{
	if (instance != nullptr)
	{
		RELEASE(instance);
	}
}

SceneProperties* SceneProperties::instance = nullptr;

#pragma endregion Scene Properties Singleton Struct


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{

}

bool ModuleScene::Init()
{
	sProps = sProps->Instance();

	return true;
}

bool ModuleScene::Start()
{
	//Import Example mesh & texture
	GameObject* aux = MeshImporter::ImportMesh("../Output/Assets/BakerHouse.fbx");

	std::vector<CompMeshRenderer*> auxMesh = aux->GetComponentsInChildrens<CompMeshRenderer>(MESH_RENDERER);

	Texture auxTexture;
	auxTexture.id = TextureImporter::ImportTexture("../Output/Assets/Baker_house.png");
	auxTexture.path = "../Output/Assets/Baker_house.png";

	for (int i = 0; i < auxMesh.size(); ++i)
	{
		auxMesh[i]->GetMesh()->SetTexture(auxTexture);
	}
	///
	return true;
}

bool ModuleScene::CleanUp()
{
	RELEASE(sProps->root);
	sProps->Delete();

	return true;
}

UpdateStatus ModuleScene::PreUpdate()
{


	return UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::Update()
{

	//Update Game Objects
	UpdateGameObject(sProps->root);


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