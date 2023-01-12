#include "ModuleResources.h"

#include "ModuleFileSystem.h"

#include "LSUUID.h"

#include "ResourceTexture.h"
#include "ResourceModel.h"
#include "ResourceShader.h"
#include "ResourceMaterial.h"
#include "Material.h"
#include "LibraryManager.h"
#include "LibraryFolder.h"

#include "TextureImporter.h"
#include "MeshImporter.h"
#include "ShaderManager.h"

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


Resource* ResourceProperties::CreateNewResource(std::string assetsPath, RESOURCE_TYPE type)
{
	Resource* toReturn = nullptr;
	std::string uuid = LS_UUID::Generate();

	switch (type)
	{
		case RESOURCE_TYPE::TEXTURE: toReturn = new ResourceTexture(uuid); break;
		case RESOURCE_TYPE::MODEL: toReturn = new ResourceModel(uuid); break;
		case RESOURCE_TYPE::SHADER: toReturn = new ResourceShader(uuid); break;
		case RESOURCE_TYPE::MATERIAL: toReturn = new ResourceMaterial(uuid); break;
		default: toReturn = new Resource(uuid, RESOURCE_TYPE::UNKNOWN); break;
	}

	if (toReturn != nullptr)
	{
		toReturn->SetAssetsFile(assetsPath);
	}

	return toReturn;
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

	resProps->requestFullFolderFileCheck = true;

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
			res.second->CleanInstance();
			RELEASE(resProps->resources[res.first]);
		}
	}
	resProps->resources.clear();

	RELEASE(resProps);

	ShaderManager::Shutdown();
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

	if (resProps->requestFullFolderFileCheck)
	{
		FolderFileCheck(fsProps->rootFolder, true);
		resProps->requestFullFolderFileCheck = false;
	}

	//Delete Plan delete
	if (!resProps->planDeleteLib.empty())
	{
		for (int i = 0; i < resProps->planDeleteLib.size(); ++i)
		{
			resProps->planDeleteLib[i]->CleanInstance();
			RELEASE(resProps->planDeleteLib[i]);

		}
		resProps->planDeleteLib.clear();
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

void ModuleResources::ImportFile(Resource* resource)
{
	switch (resource->GetType())
	{
		case RESOURCE_TYPE::TEXTURE: TextureImporter::ImportToLibrary((ResourceTexture*)resource); break;
		case RESOURCE_TYPE::MODEL: MeshImporter::ImportToLibrary((ResourceModel*)resource); break;
		case RESOURCE_TYPE::SHADER: ShaderManager::ImportToLibrary((ResourceShader*)resource); break;
		case RESOURCE_TYPE::MATERIAL: 
			ResourceMaterial* aux = (ResourceMaterial*)resource;
			aux->ImportToLibrary();
				break;
	}

	resource->Save();
}


void ModuleResources::UnloadResource(Resource* resource)
{
	std::string uuid = resource->GetUUID();

	RELEASE(resource);
	resProps->resources.erase(uuid);
}

void ModuleResources::FolderFileCheck(LibraryFolder* folder, bool fullCheck)
{
	for (int i = 0; i < folder->libItem.size(); ++i)
	{
		if (folder->libItem[i]->resUuid.empty())
		{
			Resource* res = resProps->CreateNewResource(folder->libItem[i]->path, GetResourceType(folder->libItem[i]->extension));
			//Check for meta, if it doesn't has meta file, create it.
			if (!folder->libItem[i]->hasMeta)
			{
				folder->libItem[i]->hasMeta = true;
			}
			else
			{
				//Load meta file
				res->Load();
			}
			
			//Import the file if the meta doesn't have a direction to the library file or the library file can't be found.
			if (res->GetLibraryFile().empty() || !LibraryManager::Exists(res->GetLibraryFile()))
			{
				res->CleanInstance();
				ImportFile(res); //Imports to lib
			}
			resProps->resources[res->GetUUID()] = res;

			folder->libItem[i]->resUuid = res->GetUUID();
		}
	}

	if (fullCheck)
	{
		for (int k = 0; k < folder->children.size(); ++k)
		{
			FolderFileCheck(folder->children[k], fullCheck);
		}
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
		case str2int("dae"):
		case str2int("DAE"):
			return RESOURCE_TYPE::MODEL;
		case str2int("mh"):
			return RESOURCE_TYPE::MESH;
		case str2int("sc"):
		case str2int("SC"):
			return RESOURCE_TYPE::SCENE;
		case str2int("lss"):
		case str2int("LSS"):
		case str2int("shader"):
		case str2int("SHADER"):
			return RESOURCE_TYPE::SHADER;
		case str2int("material"):
		case str2int("MATERIAL"):
		case str2int("mat"):
		case str2int("MAT"):
			return RESOURCE_TYPE::MATERIAL;
		default:
			return RESOURCE_TYPE::UNKNOWN;
	}
}