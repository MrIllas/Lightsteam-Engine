#include "Application.h"

#include "Loggs.h"

//Modules
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"
#include "ModuleResources.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "GameTime.h"


#pragma region Time
Time::Time()
{
}

Time* Time::Instance()
{
	if (G_Time == nullptr) G_Time = new Time();
	return G_Time;
}

void Time::Delete()
{
	if (G_Time != nullptr)
	{
		RELEASE(G_Time);
	}
}

float Time::GetActiveDeltaTime()
{
	if (GameTime::IsStopped()) return frameTime;
	else return GameTime::GetDeltaTime();
}

Time* Time::G_Time = nullptr;
#pragma endregion Time Singleton struct

Application::Application()
{
	window = new ModuleWindow(this, true);
	input = new ModuleInput(this, true);
	fileSystem = new ModuleFileSystem(this, true);
	resources = new ModuleResources(this, true);
	camera = new ModuleCamera3D(this, true);
	scene = new ModuleScene(this, true);
	editor = new ModuleEditor(this, true);
	renderer3D = new ModuleRenderer3D(this, true);
	
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(fileSystem);
	AddModule(resources);
	AddModule(camera);
	AddModule(editor);
	AddModule(scene);
	
	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
}

bool Application::Init()
{
	bool ret = true;

	G_Time = Time::Instance();
	Loggs::Instance();

	LOG(LOG_TYPE::ENGINE, "-------------- Initializing Modules --------------");
	// Call Init() in all modules
	for (int i = 0, count = list_modules.count() ; i <count ; i++)
	{
		list_modules[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG(LOG_TYPE::ENGINE, "-------------- Starting Modules --------------");

	for (int i = 0, count = list_modules.count(); i < count; i++)
	{
		list_modules[i]->Start();
	}

	//Load Editor Configuration
	LoadEditorConfiguration();

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	//ms_timer.Start();
	GameTime::Update(G_Time->frameTime);
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
UpdateStatus Application::Update()
{
	UpdateStatus ret = UPDATE_CONTINUE;
	if (isStopping) ret = UPDATE_STOP;
	PrepareUpdate();

	for (int i = 0, count = list_modules.count(); i < count && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PreUpdate();
	}

	for (int i = 0, count = list_modules.count(); i < count && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->Update();
	}

	for (int i = 0, count = list_modules.count(); i < count && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PostUpdate();
	}

	G_Time->deltaTime = timer.getDeltaTime();
	G_Time->frameTime = 1.0f / G_Time->frameCap;
	if (G_Time->deltaTime < G_Time->frameTime)
	{
		float sleepTime = (G_Time->frameTime - G_Time->deltaTime) * 1000;
		Sleep(sleepTime);
	}
	
	timer.Reset();

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (int i = list_modules.count() -1; i >= 0 && ret; i--)
	{
		ret = list_modules[i]->CleanUp();
	}

	for (int i = list_modules.count() - 1; i >= 0; i--)
	{
		RELEASE(list_modules[i]);
	}

	//Cleans the global Timer
	G_Time->Delete();

	return ret;
}

void Application::LoadEditorConfiguration()
{

	pugi::xml_document config;

	pugi::xml_parse_result result = config.load_file(CONFIG_FILENAME);

	for (int i = 0, count = list_modules.count(); i < count; i++)
	{
		pugi::xml_node data = config.child("config").child(list_modules[i]->name.c_str());
		list_modules[i]->LoadSettingsData(data);
		
	}

	//Load fps cap
	G_Time->frameCap = config.first_child().child("Application").child("FpsCap").attribute("value").as_int();
	
}

void Application::SaveEditorConfiguration()
{
	pugi::xml_document config;

	pugi::xml_parse_result result = config.load_file(CONFIG_FILENAME);

	for (int i = 0, count = list_modules.count(); i < count; i++)
	{
		pugi::xml_node data = config.first_child().child(list_modules[i]->name.c_str());
		list_modules[i]->SaveSettingsData(data);

	}

	//Save fps cap
	config.first_child().child("Application").child("FpsCap").attribute("value") = G_Time->frameCap;

	config.save_file(CONFIG_FILENAME);

}

void Application::StopEngine()
{
	isStopping = true;

	LOG(LOG_TYPE::NONE, "'||'''|,                      |'   .         .   `| ");
	LOG(LOG_TYPE::NONE, " ||   ||                     |'  .| |,     .| |,  `|");
	LOG(LOG_TYPE::NONE, " ||;;;;   '||  ||` .|''|,    |   |   |     |   |   |");
	LOG(LOG_TYPE::NONE, " ||   ||   `|..||  ||..||    |,                   ,|");
	LOG(LOG_TYPE::NONE, ".||...|'       ||  `|...      |.                 .|");
	LOG(LOG_TYPE::NONE, "            ,  |'                      ....        ");
	LOG(LOG_TYPE::NONE, "             ''                                      ");

	SaveEditorConfiguration();
}