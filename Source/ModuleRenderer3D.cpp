#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "CompCamera.h"
#include "ShaderManager.h"
#include "Renderer.h"

#pragma region RenderProperties

RenderProperties::RenderProperties()
{
	worldLight = new Light();
}

RenderProperties* RenderProperties::Instance()
{
	if (rProps == nullptr) rProps = new RenderProperties();

	return rProps;
}

void RenderProperties::Delete()
{
	if (rProps != nullptr)
	{
		RELEASE(rProps);
	}
}

void RenderProperties::ToggleVsync() { SDL_GL_SetSwapInterval(vsync); }
void RenderProperties::ToggleWireframe() { wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
void RenderProperties::ToggleDepthTest() { depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST); }
void RenderProperties::ToggleCullFace() { cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE); }
void RenderProperties::ToggleLighting() { lighting ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING); }
void RenderProperties::ToggleFog() { fog ? glEnable(GL_FOG) : glDisable(GL_FOG); }
void RenderProperties::ToggleColorMaterial() { colorMaterial ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL); }
void RenderProperties::ToggleTexture2D() { texture2D ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D); }

RenderProperties* RenderProperties::rProps = nullptr;

#pragma endregion Render Properties Singleton Struct

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Renderer3D";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG(LOG_TYPE::ENGINE, "INIT: 3D Renderer context");
	bool ret = true;
	
	//Set context attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//Create context
	context = SDL_GL_CreateContext(WindowProperties::Instance()->window);
	if(context == NULL)
	{
		LOG(LOG_TYPE::ERRO, "OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	//Initialize glew
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Glew could not be initialized! Glew_error: %s \n", glewGetErrorString(error));
	}
	else
	{
		LOG(LOG_TYPE::ENGINE, "INIT: Glew %s, using OpenGL %s", glewGetString(GLEW_VERSION), glGetString(GL_VERSION));
	}
	
	if(ret == true)
	{
		//Get instance of WindowProperties
		wProps = WindowProperties::Instance();

		//Render properties singleton
		rProps = RenderProperties::Instance();

		//Camera properties singleton
		cProps = CameraProperties::Instance();

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG(LOG_TYPE::ERRO, "ERROR: Initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG(LOG_TYPE::ERRO, "ERROR: Initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG(LOG_TYPE::ERRO, "ERROR: Initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		/*lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.60f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();*/
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		//lights[0].Active(true);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// FOG
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 1.0f);
		glFogf(GL_FOG_END, 30.0f);
		float fogC[] = { 0.5, 0.5, 0.5, 1.0 };
		glFogfv(GL_FOG_COLOR, fogC);

		glEnable(GL_SCISSOR_TEST);
	}
	// Projection matrix for

	OnResize(wProps->w, wProps->h);

	ShaderManager::Init();

	return ret;
}

// PreUpdate: clear buffer
UpdateStatus ModuleRenderer3D::PreUpdate()
{
	//Cleaning
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	/*glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cProps->editorCamera.GetViewMatrix());

	lights[0].SetPos(cProps->editorCamera.Position.x, cProps->editorCamera.Position.y, cProps->editorCamera.Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();*/

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UpdateStatus ModuleRenderer3D::PostUpdate()
{
	//Meshes
	cProps->editorCamera.renderer->Render(false);
	
	if (cProps->gameCameras.size() != 0)
		if (cProps->gameCameras.at(cProps->mainCameraId)->camera.renderer != nullptr)
			cProps->gameCameras.at(cProps->mainCameraId)->camera.renderer->Render();


	//Swap Buffer
	SDL_GL_SwapWindow(wProps->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	//Render Properties Struct singleton
	RELEASE(rProps->worldLight);
	RenderProperties::Delete();

	LOG(LOG_TYPE::ENGINE, "Destroying 3D Renderer");
	if (context != NULL)
	{
		SDL_GL_DeleteContext(context);
	}

	ShaderManager::Shutdown();

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();*/
	//rProps->ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 2048.0f);
	//rProps->ProjectionMatrix = rProps->ProjectionMatrix.OpenGLPerspProjRH(0.125f, 2048.0f, 60.f, (float)width / (float)height);

	//glLoadMatrixf(&rProps->ProjectionMatrix); //Without shaders

	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
}

#pragma region Save/Load Settings

void ModuleRenderer3D::LoadSettingsData(pugi::xml_node& load)
{
	rProps->vsync = load.child("Vsync").attribute("value").as_bool();
	rProps->wireframe = load.child("Wireframe").attribute("value").as_bool();
	rProps->depthTest = load.child("DepthTest").attribute("value").as_bool();
	rProps->cullFace = load.child("CullFace").attribute("value").as_bool();
	rProps->lighting = load.child("Lighting").attribute("value").as_bool();
	rProps->fog = load.child("Fog").attribute("value").as_bool();
	rProps->colorMaterial = load.child("ColorMaterial").attribute("value").as_bool();
	rProps->texture2D = load.child("Texture2D").attribute("value").as_bool();

	rProps->ToggleVsync();
	rProps->ToggleWireframe();
	rProps->ToggleDepthTest();
	rProps->ToggleCullFace();
	rProps->ToggleLighting();
	rProps->ToggleFog();
	rProps->ToggleColorMaterial();
	rProps->ToggleTexture2D();
}

void ModuleRenderer3D::SaveSettingsData(pugi::xml_node& save)
{
	save.child("Vsync").attribute("value") = rProps->vsync;
	save.child("Wireframe").attribute("value") = rProps->wireframe;
	save.child("Fog").attribute("value") = rProps->fog;
	save.child("DepthTest").attribute("value") = rProps->depthTest;
	save.child("CullFace").attribute("value") = rProps->cullFace;
	save.child("Lighting").attribute("value") = rProps->lighting;
	save.child("ColorMaterial").attribute("value") = rProps->colorMaterial;
	save.child("Texture2D").attribute("value") = rProps->texture2D;
}

#pragma endregion Save & Load of Settings