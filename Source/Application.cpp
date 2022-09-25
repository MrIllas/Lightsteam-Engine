#include "Application.h"

//Modules
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"

Application* Application::APP = nullptr;

Application::Application()
{
	window = new ModuleWindow(this, true);
	input = new ModuleInput(this, true);
	camera = new ModuleCamera3D(this, true);
	editor = new ModuleEditor(this, true);
	renderer3D = new ModuleRenderer3D(this, true);
	
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(camera);
	AddModule(editor);
	
	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (int i = 0, count = list_modules.count() ; i <count ; i++)
	{
		list_modules[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (int i = 0, count = list_modules.count(); i < count; i++)
	{
		list_modules[i]->Start();
	}

	//ms_timer.Start();
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

	deltaTime = timer.getDeltaTime();

	if (deltaTime < fps)
	{
		float sleepTime = (fps - deltaTime) * 1000;
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

	return ret;
}

void Application::StopEngine()
{
	isStopping = true;
}