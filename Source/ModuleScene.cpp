#include "ModuleScene.h"

#include "Application.h"
#include "ModuleInput.h"

#include "LibraryManager.h"

#include "ModuleCamera3D.h"

#include "GameObject.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "CompTexture.h"
#include "CompMeshRenderer.h"

#include <fstream>
#include "ImGui/imgui_internal.h"
#include "ImGuizmo/ImGuizmo.h"

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

void SceneProperties::UnselectGO()
{
	GameObject* currentSelected = GetSelectedGO(root);
	if (currentSelected != nullptr) currentSelected->selected = false;
}

bool SceneProperties::Intersect(GameObject* go, LineSegment ray)
{
	CompMeshRenderer* meshRenderer = go->GetComponent<CompMeshRenderer>(MESH_RENDERER);
	if (meshRenderer != nullptr)
	{
		if (meshRenderer->GetAABB().Intersects(ray))
		{
			UnselectGO(); //Unselects current selected GO;
			go->selected = true;
			return true;
		}
	}
	

	//Iteration and check of childrens
	if (go->HasChildren())
	{
		for (int i = 0; i < go->children.size(); ++i)
		{
			if (Intersect(go->children[i], ray))
			{
				return true;
			}
		}
	}
	return false;
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
	cProps = cProps->Instance();

	return true;
}
 
bool ModuleScene::Start()
{
	////Import Example mesh & texture
	GameObject* aux = MeshImporter::ImportMesh("Assets/BakerHouse.fbx");
	//GameObject* aux = MeshImporter::ImportMesh("Assets/street/Street environment_V01.FBX");
	//std::vector<GameObject*> vGO = aux->GetChildrens();

	/*for (int i = 0; i < vGO.size(); ++i)
	{
		vGO[i]->GetComponent<CompTexture>(MATERIAL)->SetTexture
		(TextureImporter::ImportTexture("Assets/street/Building_V01_C.png"));
	}*/

	///
	


	//MeshImporter::ImportMesh("Assets/Fence.fbx");


	if (sProps->root == nullptr) return UPDATE_CONTINUE;
	InitGameObjects(sProps->root);

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
	SceneControls();

	return UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::Update()
{
	if (sProps->root == nullptr) return UPDATE_CONTINUE;

	//Update Game Objects
	UpdateGameObjects(sProps->root);


	return UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::PostUpdate()
{
	if(sProps->loadSceneRequest)
	{
		LoadScene(sProps->scenePath);
		sProps->loadSceneRequest = false;
		sProps->scenePath = "";
	}

	return UPDATE_CONTINUE;
}

void ModuleScene::NewScene()
{
	RELEASE(sProps->root);

	sProps->root = new GameObject("Root", false);
}

void ModuleScene::InitGameObjects(GameObject* go)
{
	go->Init();

	if (go->HasChildren())
	{
		for (int i = 0; i < go->children.size(); ++i)
		{
			InitGameObjects(go->children[i]);
		}
	}
}

void ModuleScene::UpdateGameObjects(GameObject* go)
{
	go->Update();

	if (go->HasChildren())
	{
		for (int i = 0; i < go->children.size(); ++i)
		{
			UpdateGameObjects(go->children[i]);
		}
	}
}

void ModuleScene::SceneControls()
{
	if (!cProps->isMouseOnScene) return;
	//Translate
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT)
	{
		sProps->SetGuizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
	}
	//Rotate
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT)
	{
		sProps->SetGuizmoOperation(ImGuizmo::OPERATION::ROTATE);
	}
	//Scale
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT)
	{
		sProps->SetGuizmoOperation(ImGuizmo::OPERATION::SCALE);
	}
}

#pragma region Save/Load Settings
void LoadGameObjects()
{

}

void ModuleScene::SaveScene()
{
	nlohmann::JsonData data;

	//GO
	std::vector<nlohmann::ordered_json> goPool;
	MeshImporter::SaveGameObjects(sProps->root, goPool);

	data.data.emplace("GameObjects", goPool);

	//Save to JSON
	LibraryManager::SaveJSON("Library/Scenes/Test.sc", data.data.dump(4));
}

void ModuleScene::LoadScene(std::string filePath)
{
	NewScene();//Cleans scene

	nlohmann::JsonData data;

	try
	{
		char* buffer= nullptr;

		uint size = LibraryManager::Load(filePath, &buffer);
		data.data = nlohmann::ordered_json::parse(buffer, buffer + size);
		RELEASE(buffer);
	}
	catch (nlohmann::json::parse_error & ex)
	{
		LOG(LOG_TYPE::ERRO, "Error: Scene parse at byte %i: %s", ex.byte, ex.what());
	}

	std::vector<nlohmann::ordered_json> aux;
	aux = data.GetJsonVector("GameObjects");

	if (aux.size() > 0)
	{
		std::map<std::string, GameObject*> sceneMap;

		//Load scene root
		nlohmann::JsonData rootData;
		rootData.data = aux.at(0);
		sProps->root->Load(rootData);

		//Load all other GO
		for (int i = 1; i < aux.size(); ++i)
		{
			nlohmann::JsonData goData;
			goData.data = aux.at(i);

			std::string parentUUID(goData.GetString("Parent_UUID"));

			GameObject* go = new GameObject("", false);
			go->Load(goData);

			if (sceneMap.count(parentUUID) == 1)
			{
				sceneMap[parentUUID]->AddChildren(go);
			}
			else sProps->root->AddChildren(go);
			

			sceneMap.insert({ go->uuid, go });
		}
	}

}

void ModuleScene::LoadSettingsData(pugi::xml_node& load)
{

}

void ModuleScene::SaveSettingsData(pugi::xml_node& save)
{

}
#pragma endregion Save & Load of Settings