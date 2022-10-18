#pragma once
#include "Module.h"

class MeshImporter;

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Init() override;
	bool Start() override;

	bool CleanUp() override;
	
	UpdateStatus PreUpdate();
	UpdateStatus Update();
	UpdateStatus PostUpdate();

	void SaveSettingsData(pugi::xml_node& save) override;
	void LoadSettingsData(pugi::xml_node& load) override;

	void DragAndDrop(std::string path);

public:
	MeshImporter* meshImp = nullptr;
};

