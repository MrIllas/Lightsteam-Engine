#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "List.h"
#include "Timer.hpp"


#define CONFIG_FILENAME "config.xml"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleFileSystem;
class ModuleCamera3D;
class ModuleEditor;

class ModuleRenderer3D;

struct Time
{
	int frameCap = 60;
	float frameTime = 1.0f / frameCap;
	float deltaTime = 0;

	Time();
	
	static Time* Instance();

	static void Delete();

private:
	static Time* G_Time;
};

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleFileSystem* fileSystem = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;

	float fps = 1.0f / 60.0f;
	Timer timer;

private:
	float deltaTime = 0;
	List<Module*> list_modules;

	bool isStopping = false;

	Time* G_Time = nullptr;

public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

	void StopEngine();

	static Application* Instance();

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void SaveEditorConfiguration();
	void LoadEditorConfiguration();

};

#endif // !__APPLICATION_H__