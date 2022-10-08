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
	bool ret = true;

	meshImp = new MeshImporter();

	return ret;
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

#pragma region Save/Load Settings
void ModuleFileSystem::LoadSettingsData(pugi::xml_node& load)
{

}

void ModuleFileSystem::SaveSettingsData(pugi::xml_node& save)
{

}
#pragma endregion Save & Load of Settings