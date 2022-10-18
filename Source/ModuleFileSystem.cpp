#include "Application.h"
#include "ModuleFileSystem.h"

#include "PhysFS/include/physfs.h"

#include "MeshImporter.h"

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
	MeshImporter::ImportMesh(path, true);
}

#pragma region Save/Load Settings
void ModuleFileSystem::LoadSettingsData(pugi::xml_node& load)
{

}

void ModuleFileSystem::SaveSettingsData(pugi::xml_node& save)
{

}
#pragma endregion Save & Load of Settings