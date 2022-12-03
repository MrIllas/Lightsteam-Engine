#pragma once
#include "Module.h"

#include "Resource.h"

#include <map>


class LibraryFodler;
struct FileSystemProperties;

struct ResourceProperties
{
	ResourceProperties();

	static ResourceProperties* Instance();

	static void Delete();

private:
	static ResourceProperties* instance;

public:
	std::map<std::string, Resource*> resources;

	bool requestFolderFileCheck = false;
	bool requestFullFolderFileCheck = false;
	
};

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool Init() override;
	bool Start() override;

	bool CleanUp() override;

	UpdateStatus PreUpdate();
	UpdateStatus Update();
	UpdateStatus PostUpdate();

	std::string Find(std::string assetsFile);
	void ImportFile(Resource* resource);
	
	void ReleaseResource(std::string uuid);


	void FolderFileCheck(LibraryFolder* folder, bool fullCheck = false);
	void ImportToLibrary(LibraryFolder* folder);

private:
	RESOURCE_TYPE GetResourceType(std::string extension);

private:
	Resource* CreateNewResource(std::string assetsPath, RESOURCE_TYPE type);
	void UnloadResource(Resource* resource);

private:
	ResourceProperties* resProps = nullptr;
	FileSystemProperties* fsProps = nullptr;
};

