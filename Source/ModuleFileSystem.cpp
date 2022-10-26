#include "Application.h"
#include "ModuleFileSystem.h"

#include "PhysFS/include/physfs.h"

#include "MeshImporter.h"
#include "TextureImporter.h"

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

	return true;
}

bool ModuleFileSystem::Start()
{
	bool ret = true;

	meshImp->Start();

	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	bool ret = true;

	meshImp->CleanUp();
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
			MeshImporter::ImportMesh(path, nullptr, true);
			break;
		case str2int("png"):
			
			break;
	}	
}

#pragma region Save/Load Settings
void ModuleFileSystem::LoadSettingsData(pugi::xml_node& load)
{

}

void ModuleFileSystem::SaveSettingsData(pugi::xml_node& save)
{

}
#pragma endregion Save & Load of Settings