#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"

#include "SegmentAbout.h"
#include "SegmentConfiguration.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "RapidJson/document.h"
#include "RapidJson/writer.h"
#include "RapidJson/stringbuffer.h"
#include <iostream>

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	//Flags
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantCaptureMouse = true;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	return true;
}

bool ModuleEditor::Start()
{
	ImGui_ImplSDL2_InitForOpenGL(App->window->GetSDLWindow(), App->renderer3D->GetGLContext());
	ImGui_ImplOpenGL3_Init();

	//Generate the segments
	segAbout = new SegmentAbout("About", true);
	segConfiguration = new SegmentConfiguration("Configuration", true);

	return true;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	//Clean Segments
	RELEASE(segAbout);
	RELEASE(segConfiguration);

	return true;
}

UpdateStatus ModuleEditor::PostUpdate()
{
	DrawEditorGui();

	return UPDATE_CONTINUE;
}

void ModuleEditor::DrawEditorGui()
{
	BeginRender();

	//Menus
	MainMenuBar();
	UpdateSegments();

	//Demo
	ImGui::ShowDemoWindow();

	EndRender();
}

#pragma region EditorRender
void ModuleEditor::BeginRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();// App->window->GetSDLWindow());
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void ModuleEditor::EndRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}
#pragma endregion Editor Render methods

#pragma region GUI
void ModuleEditor::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Fullscreen Borderless", "F11") || App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
			{
				App->window->ToggleFullScreen();
			}

			if (ImGui::MenuItem("Exit", "ALT+F4"))
			{
				App->StopEngine();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "F1", &showAppConsole) || App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			{
				/*if (ImGui::Begin("Console", showAppConsole))
				{

					ImGui::End();
				}*/
			}

			if (ImGui::MenuItem("Configuration", "F4", &segConfiguration->enabled) || App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", "F12", &segAbout->enabled) || App->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN)
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ModuleEditor::UpdateSegments()
{
	if (segAbout->enabled)
	{
		segAbout->Update();
	}
	
	if (segConfiguration->enabled)
	{
		segConfiguration->Update();
	}
}
#pragma endregion Gui Elements of the editor