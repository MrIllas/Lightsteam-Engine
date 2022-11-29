#include "ModuleResources.h"

#include "ModuleFileSystem.h"

#include "LSUUID.h"

#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "LibraryManager.h"
#include "LibraryFolder.h"

#pragma region ResourceProperties
ResourceProperties::ResourceProperties()
{

}

ResourceProperties* ResourceProperties::Instance()
{
	if (instance == nullptr) instance = new ResourceProperties();

	return instance;
}

void ResourceProperties::Delete()
{
	if (instance != nullptr)
	{
		RELEASE(instance);
	}
}

ResourceProperties* ResourceProperties::instance = nullptr;
#pragma endregion Resource Properties singleton struct


ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Resources";
}

ModuleResources::~ModuleResources()
{
	
}

bool ModuleResources::Init()
{
	resProps = ResourceProperties::Instance();
	fsProps = FileSystemProperties::Instance();
	

	return true;
}

bool ModuleResources::Start()
{
	return true;
}

bool ModuleResources::CleanUp()
{
	//Delete Resources from memory
	for (auto const& res : resProps->resources)
	{
		if (res.second != nullptr)
		{
			RELEASE(resProps->resources[res.first]);
		}
	}
	resProps->resources.clear();

	RELEASE(resProps);
	return true;
}

UpdateStatus ModuleResources::PreUpdate()
{
	if (resProps->requestFolderFileCheck)
	{
		FolderFileCheck(fsProps->currentFolder);
		resProps->requestFolderFileCheck = false;
	}


	return UPDATE_CONTINUE;
}

UpdateStatus ModuleResources::Update()
{

	return UPDATE_CONTINUE;
}

UpdateStatus ModuleResources::PostUpdate()
{

	return UPDATE_CONTINUE;
}

std::string ModuleResources::Find(std::string assetsFile)
{
	std::string toReturn = "";


	return toReturn;
}

std::string ModuleResources::ImportFile(std::string assetsFile)
{
	std::string toReturn = "";

	Resource* resource = CreateNewResource(assetsFile, RESOURCE_TYPE::UNKNOWN);



	return toReturn;
}

Resource* ModuleResources::RequestResource(std::string uuid)
{
	std::map<std::string, Resource*>::iterator it = resProps->resources.find(uuid);
	if (it != resProps->resources.end())
	{
		it->second->referenceCount++;
		return it->second;
	}

	return nullptr; //Should try to load from Library.
}

Resource* ModuleResources::CreateNewResource(std::string assetsPath, RESOURCE_TYPE type)
{
	Resource* toReturn = nullptr;
	std::string uuid = LS_UUID::Generate();

	switch (type)
	{
		case RESOURCE_TYPE::TEXTURE: toReturn = new ResourceTexture(uuid); break;
		case RESOURCE_TYPE::MESH: toReturn = new ResourceMesh(uuid); break;
		default: toReturn = new Resource(uuid, RESOURCE_TYPE::UNKNOWN); break;
	}

	if (toReturn != nullptr)
	{
		resProps->resources[uuid] = toReturn;
		toReturn->SetAssetsFile(assetsPath);
		//Lib path set on asset's constructor
		//toReturn->SetLibraryFile("");
	}

	return toReturn;
}

void ModuleResources::FolderFileCheck(LibraryFolder* folder)
{

	for (int i = 0; i < folder->libItem.size(); ++i)
	{
		//Check for meta, if it doesn't has meta file, create it.
		if (!folder->libItem[i]->hasMeta)
		{
			Resource* r = CreateNewResource(folder->libItem[i]->path, GetResourceType(folder->libItem[i]->extension));
			if (r == nullptr) continue;
			folder->libItem[i]->hasMeta = true;

			if (r->GetType() == RESOURCE_TYPE::TEXTURE || r->GetType() == RESOURCE_TYPE::MESH)
			{
				std::string savePath = folder->libItem[i]->path;
				savePath += ".meta";
				r->Save(savePath);
			}
			
		}
			
	}
}

RESOURCE_TYPE ModuleResources::GetResourceType(std::string extension)
{
	switch (str2int(extension.c_str()))
	{
		case str2int("dds"):
		case str2int("png"):
		case str2int("tga"):
			return RESOURCE_TYPE::TEXTURE;
		case str2int("fbx"):
			return RESOURCE_TYPE::MESH;
		case str2int("sc"):
			return RESOURCE_TYPE::SCENE;
		default:
			return RESOURCE_TYPE::UNKNOWN;
	}
}