#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

WindowProperties::WindowProperties()
{
	title = "Lightsteam Engine";
	x = SDL_WINDOWPOS_CENTERED;
	y = SDL_WINDOWPOS_CENTERED;
	w = 1600;
	h = 900;
	wMin = 640;
	hMin = 360;
	wMax = 1920;
	hMax = 1080;
	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
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
		RELEASE(wProps);
	}
}

WindowProperties* WindowProperties::wProps = nullptr;

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = nullptr;
	screen_surface = nullptr;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	wProps = WindowProperties::Instance();

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		//Window Creation
		window = SDL_CreateWindow(wProps->title.c_str(), wProps->x, wProps->y, wProps->w, wProps->h, wProps->flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}

		SDL_SetWindowMinimumSize(window, wProps->wMin, wProps->hMin);
	}

	return ret;
}

UpdateStatus ModuleWindow::PreUpdate()
{
	//WProps pointer
	SDL_GetWindowSize(window, &wProps->w, &wProps->h);
	SDL_SetWindowBrightness(window, wProps->brightness);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Window Properties Struct singleton
	WindowProperties::Delete();

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::ToggleFullScreen()
{
	fullScreenDesktop = !fullScreenDesktop;

	if (fullScreenDesktop)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
	}
}