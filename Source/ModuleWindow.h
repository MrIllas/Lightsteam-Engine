#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "External/SDL/include/SDL.h"

#include <string>

class Application;

struct WindowProperties
{
	std::string title;
	int x, y, w, h;
	int wMin, hMin;
	int flags;
	float ccR, ccG, ccB;

	WindowProperties();
};

class ModuleWindow : public Module
{
public:
	ModuleWindow(Application* app, bool start_enabled = true);

	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void ToggleFullScreen();

private:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	bool fullScreenDesktop = false;

public:
	SDL_Window* GetSDLWindow() { return window; }
	SDL_Surface* GetSDLSurface() { return screen_surface; }
};

#endif // __ModuleWindow_H__