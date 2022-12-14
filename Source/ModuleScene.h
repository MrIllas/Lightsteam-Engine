#pragma once
#include "Module.h"

//#include "GameObject.h"
#include <vector>
#include "MathGeoLib/include/Geometry/LineSegment.h"

class GameObject;
class CompCamera;
class MeshRenderer;
struct CameraProperties;

struct SceneProperties
{
public:
	GameObject* root = nullptr;

	SceneProperties();

	static SceneProperties* Instance();

	static void Delete();

	GameObject* GetSelectedGO(GameObject* go = nullptr);
	void UnselectGO();

	bool Intersect(GameObject* go, LineSegment ray);

	int GetGuizmoOperation() { return guizmoOperation;  }
	void SetGuizmoOperation(int value) { guizmoOperation = value; }

	bool loadSceneRequest = false;
	std::string scenePath = "";

private:
	static SceneProperties* instance;

	int guizmoOperation = 7;//Translate
};

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init() override;
	bool Start() override;

	bool CleanUp() override;

	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;

	void SaveSettingsData(pugi::xml_node& save) override;
	void LoadSettingsData(pugi::xml_node& load) override;

	void NewScene();

	void SaveScene();
	void LoadScene(std::string filePath);

private:
	void InitGameObjects(GameObject* go);
	void UpdateGameObjects(GameObject* go);

	//void SaveGameObjects(GameObject* go, nlohmann::JsonData data);

	void SceneControls();

private:
	SceneProperties* sProps = nullptr;
	CameraProperties* cProps = nullptr;

};

