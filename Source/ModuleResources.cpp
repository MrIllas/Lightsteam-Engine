#include "ModuleResources.h"

#include "ModuleFileSystem.h"

#include "LSUUID.h"

#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "LibraryManager.h"
#include "LibraryFolder.h"

#include "TextureImporter.h"
#include "MeshImporter.h"

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
			res.second->Save(); //Save just in case
			RELEASE(resProps->resources[res.first]);
		}
	}
	resProps->resources.clear();

	RELEASE(resProps);
	return true;
}

UpdateStatus ModuleResources::PreUpdate()
{

	//Checks if all the files on the current folder have a meta and creates/loads Resources.
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

void ModuleResources::ImportFile(Resource* resource)
{
	//std::string toReturn = "";

	//toReturn = resource->GetUUID();
	//char* buffer = 
	switch (resource->GetType())
	{
		case RESOURCE_TYPE::TEXTURE: TextureImporter::ImportToLibrary((ResourceTexture*)resource); break;
		case RESOURCE_TYPE::MESH: MeshImporter::ImportToLibrary((ResourceMesh*)resource); break;
	}

	resource->Save();
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
		toReturn->SetAssetsFile(assetsPath);
	}

	return toReturn;
}

void ModuleResources::UnloadResource(Resource* resource)
{
	std::string uuid = resource->GetUUID();

	RELEASE(resource);
	resProps->resources.erase(uuid);
}

void ModuleResources::FolderFileCheck(LibraryFolder* folder)
{
	for (int i = 0; i < folder->libItem.size(); ++i)
	{
		if (folder->libItem[i]->resUuid.empty())
		{
			Resource* res = CreateNewResource(folder->libItem[i]->path, GetResourceType(folder->libItem[i]->extension));
			//Check for meta, if it doesn't has meta file, create it.
			if (!folder->libItem[i]->hasMeta)
			{
				//Creates meta and the file inside library
				folder->libItem[i]->hasMeta = true;

				//ImportFile(folder->libItem[i]->path, GetResourceType(folder->libItem[i]->extension));
				//folder->libItem[i]->hasMeta = true;
			}
			else
			{
				//Load meta file
				//res = CreateNewResource(folder->libItem[i]->path, GetResourceType(folder->libItem[i]->extension));
				res->Load();

			}
			
			//Import the file if the meta doesn't have a direction to the library file or the library file can't be found.
			if (res->GetLibraryFile().empty() || !LibraryManager::Exists(res->GetLibraryFile())) ImportFile(res); //Imports to lib
			resProps->resources[res->GetUUID()] = res;

			folder->libItem[i]->resUuid = res->GetUUID();
		}
	}
}

void ModuleResources::ImportToLibrary(LibraryFolder* folder)
{
	for (int i = 0; i < folder->libItem.size(); ++i)
	{

	}
}

RESOURCE_TYPE ModuleResources::GetResourceType(std::string extension)
{
	switch (str2int(extension.c_str()))
	{
		case str2int("dds"):
		case str2int("DDS"):
		case str2int("png"):
		case str2int("PNG"):
		case str2int("tga"):
		case str2int("TGA"):
			return RESOURCE_TYPE::TEXTURE;
		case str2int("fbx"):
		case str2int("FBX"):
			return RESOURCE_TYPE::MESH;
		case str2int("sc"):
		case str2int("SC"):
			return RESOURCE_TYPE::SCENE;
		default:
			return RESOURCE_TYPE::UNKNOWN;
	}
}