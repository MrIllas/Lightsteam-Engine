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
	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
	ccR = static_cast<float>(0x64) / static_cast<float>(0xFF);
	ccG = static_cast<float>(0x95) / static_cast<float>(0xFF);
	ccB = static_cast<float>(0xED) / static_cast<float>(0xFF);
}

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

	WindowProperties wProps = WindowProperties();

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
		window = SDL_CreateWindow(wProps.title.c_str(), wProps.x, wProps.y, wProps.w, wProps.h, wProps.flags);

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

		glContext = SDL_GL_CreateContext(window);
		if (glContext == NULL)
		{
			LOG("GL Context could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		SDL_GL_MakeCurrent(window, glContext);
		SDL_GL_SetSwapInterval(1); //Enable vsync

		SDL_SetWindowMinimumSize(window, wProps.wMin, wProps.hMin);
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	if (glContext != NULL)
	{
		SDL_GL_DeleteContext(glContext);
	}

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