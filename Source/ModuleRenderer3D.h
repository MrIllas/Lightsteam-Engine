#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Light.h"

#define MAX_LIGHTS 8

struct RenderProperties
{
public:
	bool vsync = false;

	RenderProperties();

	static RenderProperties* Instance();

	static void Delete();

	void ToggleVsync();

private:
	static RenderProperties* rProps;

};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	UpdateStatus PreUpdate();
	UpdateStatus PostUpdate();
	bool CleanUp();

	void SaveSettingsData(pugi::xml_node& save) override;

	void LoadSettingsData(pugi::xml_node& load) override;

	void OnResize(int width, int height);

public:
	SDL_GLContext GetGLContext() { return context; }

public:

	Light lights[MAX_LIGHTS];
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

private:
	SDL_GLContext context;

	RenderProperties* rProps = nullptr;
};

#endif // !__MODULERENDERER3D_H__