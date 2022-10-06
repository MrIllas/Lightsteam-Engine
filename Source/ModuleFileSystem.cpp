#include "ModuleFileSystem.h"

#include "AssetImporter.h"

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

	assImp = new AssetImporter();

	return ret;
}

bool ModuleFileSystem::Start()
{
	bool ret = true;

	assImp->Start();

	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	bool ret = true;

	assImp->CleanUp();
	RELEASE(assImp);

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