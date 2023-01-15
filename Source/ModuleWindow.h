#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "External/SDL/include/SDL.h"

#include <string>

class Application;

struct WindowProperties
{
public:
	std::string title;
	int x, y, w, h;
	int wMin, hMin;
	int wMax, hMax;
	int flags;
	float ccR, ccG, ccB;
	float brightness;

	WindowProperties();

	static WindowProperties* Instance();

	static void Delete();

	void ToggleBorderless();
	void ToggleFullscreen();
	void ToggleResizable();
	void ToggleFullscreenDesktop();

	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	bool fullScreenDesktop = false;
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool maximized = false;

private:
	static WindowProperties* wProps;


};

class ModuleWindow : public Module
{
public:
	ModuleWindow(Application* app, bool start_enabled = true);

	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	UpdateStatus PreUpdate();

	void SetTitle(const char* title);

	void SaveSettingsData(pugi::xml_node& save) override;

	void LoadSettingsData(pugi::xml_node& load) override;

public:
	//SDL_Window* GetSDLWindow() { return window; }
	SDL_Surface* GetSDLSurface() { return screen_surface; }

private:
	

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	WindowProperties* wProps = nullptr;

};

#endif // __ModuleWindow_H__