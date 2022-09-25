#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "List.h"
#include "Timer.hpp"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleCamera3D;
class ModuleEditor;

class ModuleRenderer3D;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;

	float fps = 1.0f/60.0f;
	Timer timer;

private:
	float deltaTime = 0;
	List<Module*> list_modules;

	bool isStopping = false;

	static Application* APP;

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

};

#endif // !__APPLICATION_H__