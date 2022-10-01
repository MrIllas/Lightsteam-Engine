#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "FrameBuffer.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma region RenderProperties

RenderProperties::RenderProperties()
{
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
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Set context attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Create context
	context = SDL_GL_CreateContext(WindowProperties::Instance()->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
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
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));
		LOG("	Vendor: %s", glGetString(GL_VENDOR));
		LOG("	Renderer: %s", glGetString(GL_RENDERER));
		LOG("	OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("	GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	
	if(ret == true)
	{
		//Get instance of WindowProperties
		wProps = WindowProperties::Instance();

		//Render properties singleton
		rProps = RenderProperties::Instance();

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
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
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.60f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		lights[0].Active(true);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// FOG
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 1.0f);
		glFogf(GL_FOG_END, 30.0f);
		float fogC[] = { 0.5, 0.5, 0.5, 1.0 };
		glFogfv(GL_FOG_COLOR, fogC);
	}
	frameBuffer = FrameBuffer::Instance();
	// Projection matrix for
		OnResize(wProps->w, wProps->h);

	return ret;
}

// PreUpdate: clear buffer
UpdateStatus ModuleRenderer3D::PreUpdate()
{
	//Color c = App->camera->g
	//Cleaning
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UpdateStatus ModuleRenderer3D::PostUpdate()
{
	//Draw 3D Graphics

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetFrameBuffer());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	


	glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);

	// front face =================
	//glVertex3f(1.0f, 1.0f, 1.0f); //0    
	//glVertex3f(-1.0, 1.0, 1.0); //1
	//glVertex3f(-1.0, -1.0, 1.0); //2

	//glVertex3f(1.0, -1.0, 1.0);  //3  
	//glVertex3f(1.0, -1.0, -1.0); //4
	//glVertex3f(1.0, 1.0, -1.0); //5
	//glVertex3f(-1.0, 1.0, -1.0); //6
	//glVertex3f(-1.0, -1.0, -1.0); //7

	// front face =================
	glColor3f(1.0f, 0.0f, 0.0f);  // Red
	glVertex3f(1.0f, 1.0f, 1.0f);    // v0-v1-v2
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);   // v2-v3-v0
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// right face =================
	glColor3f(1.0f, 0.0f, 1.0f);  // Violet
	glVertex3f(1.0f, 1.0f, 1.0f);    // v0-v3-v4
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(1.0, -1.0, -1.0);    // v4-v5-v0
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// top face ===================
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, 1.0f, 1.0f);    // v0-v5-v6
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);    // v6-v1-v0
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// back face =================
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow
	glVertex3f(-1.0, 1.0, -1.0); // 6-5-7
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0); //7-4-5
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(1.0, 1.0, -1.0);

	// left face =================
	glColor3f(0.0f, 0.0f, 1.0f);  // Blue
	glVertex3f(-1.0, 1.0, 1.0); //1-6-7
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0); //7-2-1
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);

	// Bottom face =================
	glColor3f(1.0f, 0.5f, 0.0f); // Orange
	glVertex3f(-1.0, -1.0, -1.0); //7-2-3
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0); //3-4-7
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);

	glEnd();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Swap Buffer
	SDL_GL_SwapWindow(wProps->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	//Render Properties Struct singleton
	RenderProperties::Delete();

	LOG("Destroying 3D Renderer");
	if (context != NULL)
	{
		SDL_GL_DeleteContext(context);
	}

	frameBuffer->Delete();

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 2048.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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