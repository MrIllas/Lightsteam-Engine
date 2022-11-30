#include "Application.h"
#include "ModuleFileSystem.h"

#include "PhysFS/include/physfs.h"

#include "LibraryFolder.h"
#include "LibraryManager.h"
#include "MeshImporter.h"
#include "TextureImporter.h"

#include "ModuleResources.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "CompMeshRenderer.h"
#include "CompTexture.h"


#pragma region FileSystemProperties
FileSystemProperties::FileSystemProperties()
{

}

FileSystemProperties* FileSystemProperties::Instance()
{
	if (instance == nullptr) instance = new FileSystemProperties();

	return instance;
}

void FileSystemProperties::Delete()
{
	if (instance != nullptr)
	{
		RELEASE(instance);
	}
}


FileSystemProperties* FileSystemProperties::instance = nullptr;
#pragma endregion File System Properties singleton struct

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "FileSystem";
}

ModuleFileSystem::~ModuleFileSystem()
{
	
}

bool ModuleFileSystem::Init()
{
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	

	meshImp = new MeshImporter();
	textImp = new TextureImporter();

	fsProps = FileSystemProperties::Instance();
	resProps = ResourceProperties::Instance();
	sProps = SceneProperties::Instance();

	fsProps->rootFolder = new LibraryFolder("Assets", "Assets", nullptr);
	LibraryManager::Init(*fsProps->rootFolder);

	return true;
}

bool ModuleFileSystem::Start()
{
	bool ret = true;


	meshImp->Init();

	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	bool ret = true;

	LibraryManager::CleanUp();

	meshImp->CleanUp();

	if (fsProps->rootFolder != nullptr) RELEASE(fsProps->rootFolder);
	RELEASE(fsProps);

	RELEASE(meshImp);
	RELEASE(textImp);

	return ret;
}

UpdateStatus ModuleFileSystem::PreUpdate()
{

	return UPDATE_CONTINUE;
}

UpdateStatus ModuleFileSystem::Update()
{


	return UPDATE_CONTINUE;
}

UpdateStatus ModuleFileSystem::PostUpdate()
{
	

	return UPDATE_CONTINUE;
}

void ModuleFileSystem::DragAndDrop(std::string path)
{
	std::string extension = "";

	//Find last dot
	size_t pos = path.find_last_of(".");
	//make sure the poisition is valid
	if (pos != std::string::npos)
		extension = path.substr(pos + 1);

	switch (str2int(extension.c_str()))
	{
		case str2int("fbx"):
			//MeshImporter::ImportMesh(path, nullptr, true);
			//break;
		case str2int("dds"):
		case str2int("png"):
		{
			LibraryManager::Copy(path, fsProps->currentFolder->path);

			//TextureImporter::ImportTexture(path);
			/*GameObject* aux = sProps->GetSelectedGO();

			if (aux != nullptr)
			{
				CompTexture* auxText = nullptr;
				auxText = aux->GetComponent<CompTexture>(MATERIAL);

				if (auxText != nullptr)
				{
					auxText->SetTexture(TextureImporter::ImportTexture(path));
				}
				else
				{
					LOG(LOG_TYPE::ERRO, "ERROR: The selected 'GameObject' doesn't have a 'CompTexture'");
				}
				
			}
			else
			{
				LOG(LOG_TYPE::ERRO, "ERROR: There is no 'GameObject' selected!");
			}*/
		}
			break;
		case str2int("jpg"):
			LOG(LOG_TYPE::ERRO, "ERROR: JPG format not supported!");
			break;
	}	
	LibraryManager::FolderSystemUpdate(fsProps->currentFolder);
	resProps->requestFolderFileCheck = true;
}

#pragma region Save/Load Settings
void ModuleFileSystem::LoadSettingsData(pugi::xml_node& load)
{

}

void ModuleFileSystem::SaveSettingsData(pugi::xml_node& save)
{

}
#pragma endregion Save & Load of Settings