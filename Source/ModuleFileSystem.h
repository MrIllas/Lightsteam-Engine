#pragma once
#include "Module.h"

class MeshImporter;
class TextureImporter;
class LibraryFolder;

struct SceneProperties;

struct FileSystemProperties
{
public:
	FileSystemProperties();

	static FileSystemProperties* Instance();

	static void Delete();

public:
	LibraryFolder* rootFolder = nullptr;
	LibraryFolder* currentFolder = nullptr;
private:
	static FileSystemProperties* instance;
};

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
	FileSystemProperties* fsProps = nullptr;
	SceneProperties* sProps = nullptr;

	MeshImporter* meshImp = nullptr;
	TextureImporter* textImp = nullptr;
};

