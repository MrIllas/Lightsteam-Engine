#pragma once
#include "Module.h"

#include "Resource.h"

#include <map>
#include <vector>

class LibraryFolder;
struct FileSystemProperties;

struct ResourceProperties
{
	ResourceProperties();

	static ResourceProperties* Instance();

	static void Delete();

	Resource* CreateNewResource(std::string assetsPath, RESOURCE_TYPE type);

	template <class T>
	std::vector<T*> GetResourcePool(RESOURCE_TYPE type)
	{
		std::vector<T*> toReturn;

		for (auto const& res : resources)
		{
			if (res.second->GetType() == type)
			{
				T* aux = (T*) resources.at(res.first);

				toReturn.emplace_back(aux);
			}
		}

		return toReturn;
	}


private:
	static ResourceProperties* instance;

public:
	std::map<std::string, Resource*> resources;
	std::vector<Resource*> planDeleteLib;

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

	void ImportFile(Resource* resource);
	
	void ReleaseResource(std::string uuid);


	void FolderFileCheck(LibraryFolder* folder, bool fullCheck = false);

private:
	RESOURCE_TYPE GetResourceType(std::string extension);

private:
	
	void UnloadResource(Resource* resource);

private:
	ResourceProperties* resProps = nullptr;
	FileSystemProperties* fsProps = nullptr;
};

