#pragma once
#include "Module.h"

//#include "GameObject.h"

class GameObject;


struct SceneProperties
{
public:
	GameObject* root = nullptr;

	SceneProperties();

	static SceneProperties* Instance();

	static void Delete();

	GameObject* GetSelectedGO(GameObject* go = nullptr);

private:
	static SceneProperties* instance;
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


private:
	SceneProperties* sProps = nullptr;

private:
	void UpdateGameObject(GameObject* go);
};

