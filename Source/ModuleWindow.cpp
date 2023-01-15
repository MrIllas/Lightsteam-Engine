#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#pragma region WindowProperties
WindowProperties::WindowProperties()
{
	title = "Lightsteam Engine v.";
	title += std::to_string(ENGINE_VERSION_MAJOR);
	title += ".";
	title += std::to_string(ENGINE_VERSION_MINOR);

	x = SDL_WINDOWPOS_CENTERED;
	y = SDL_WINDOWPOS_CENTERED;
	w = 1600;
	h = 900;
	wMin = 640;
	hMin = 360;
	wMax = 1920;
	hMax = 1080;
	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;//SDL_WINDOW_MAXIMIZED
	brightness = 1.0f;
}

WindowProperties* WindowProperties::Instance()
{
	if (wProps == nullptr) wProps = new WindowProperties();

	return wProps;
}

void WindowProperties::Delete()
{
	if (wProps != nullptr)
	{

		//Destroy window
		if (wProps->window != NULL)
		{
			SDL_DestroyWindow(wProps->window);
		}

		RELEASE(wProps);
	}
}

#pragma region FlagToggles
void WindowProperties::ToggleFullscreenDesktop()
{
	if (fullScreenDesktop)	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else SDL_SetWindowFullscreen(window, 0);
}

void WindowProperties::ToggleFullscreen()
{
	if (fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else SDL_SetWindowFullscreen(window, 0);
}

void WindowProperties::ToggleResizable()
{
	if (resizable) SDL_SetWindowResizable(window, SDL_TRUE);
	else SDL_SetWindowResizable(window, SDL_FALSE);
}

void WindowProperties::ToggleBorderless()
{
	if (borderless) SDL_SetWindowBordered(window, SDL_FALSE);
	else SDL_SetWindowBordered(window, SDL_TRUE);
}
#pragma endregion All flag toggles

WindowProperties* WindowProperties::wProps = nullptr;

#pragma endregion Window Properties Singelon Struct

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Window";
	screen_surface = nullptr;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	bool ret = true;

	wProps = WindowProperties::Instance();

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG(LOG_TYPE::ERRO, "SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		LOG(LOG_TYPE::ENGINE, "INIT: SDL window & surface.");
		//Window Creation
		wProps->window = SDL_CreateWindow(wProps->title.c_str(), wProps->x, wProps->y, wProps->w, wProps->h, wProps->flags);

		if(wProps->window == NULL)
		{
			LOG(LOG_TYPE::ERRO, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(wProps->window);
		}

		SDL_SetWindowMinimumSize(wProps->window, wProps->wMin, wProps->hMin);
	}

	return ret;
}

UpdateStatus ModuleWindow::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG(LOG_TYPE::ENGINE, "Destroying SDL window and quitting all SDL systems");

	//Window Properties Struct singleton
	WindowProperties::Delete();

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(wProps->window, title);
}

#pragma region Save/Load Settings

void ModuleWindow::LoadSettingsData(pugi::xml_node& load)
{
	wProps->brightness = load.child("Brightness").attribute("value").as_float();
	wProps->fullScreenDesktop = load.child("FullscreenDesktop").attribute("value").as_bool();
	wProps->fullscreen = load.child("Fullscreen").attribute("value").as_bool();
	wProps->resizable = load.child("Resizable").attribute("value").as_bool();
	wProps->borderless = load.child("Borderless").attribute("value").as_bool();
	wProps->maximized = load.child("Maximized").attribute("value").as_bool();

	if (wProps->maximized) SDL_MaximizeWindow(wProps->window);
	else
	{
		wProps->w = load.child("Resolution").attribute("Width").as_int();
		wProps->h = load.child("Resolution").attribute("Height").as_int();
		SDL_SetWindowSize(wProps->window, wProps->w, wProps->h);
		SDL_SetWindowPosition(wProps->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	//SDL_SetWindowBrightness(wProps->window, wProps->brightness);
	wProps->ToggleBorderless();
	wProps->ToggleFullscreen();
	wProps->ToggleResizable();
	wProps->ToggleFullscreenDesktop();
}

void ModuleWindow::SaveSettingsData(pugi::xml_node& save)
{
	save.child("Brightness").attribute("value") = wProps->brightness;
	save.child("FullscreenDesktop").attribute("value") = wProps->fullScreenDesktop;
	save.child("Fullscreen").attribute("value") = wProps->fullscreen;
	save.child("Resizable").attribute("value") = wProps->resizable;
	save.child("Borderless").attribute("value") = wProps->borderless;
	save.child("Maximized").attribute("value") = SDL_GetWindowFlags(wProps->window) & SDL_WINDOW_MAXIMIZED ? true : false;

	SDL_GetWindowSize(wProps->window, &wProps->w, &wProps->h);
	save.child("Resolution").attribute("Width") = wProps->w;
	save.child("Resolution").attribute("Height") = wProps->h;
}

#pragma endregion Save & Load of Settings