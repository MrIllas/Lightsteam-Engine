#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"

#include "Segment.h"
#include "SegmentAbout.h"
#include "SegmentConfiguration.h"
#include "SegmentConsole.h"
#include "SegmentScene.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#pragma region EditorProperties
EditorProperties::EditorProperties()
{

}

EditorProperties* EditorProperties::Instance()
{
	if (instance == nullptr) instance = new EditorProperties();

	return instance;
}

void EditorProperties::Delete()
{
	if (instance != nullptr)
	{
		RELEASE(instance);
	}
}

void EditorProperties::SwitchColorMode() { 
	switch (colorMode)
	{
		case COLORMODE::LightMode: ImGui::StyleColorsLight(); break;
		case COLORMODE::DarkMode: ImGui::StyleColorsDark(); break;
		case COLORMODE::ClassicMode: ImGui::StyleColorsClassic(); break;
	}
}

EditorProperties* EditorProperties::instance = nullptr;

#pragma endregion Editor Properties Singleton Struct

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Editor";
	eProps = EditorProperties::Instance();
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
	//Create Segments
	segments.emplace_back(new SegmentConfiguration());
	segments.emplace_back(new SegmentConsole());
	segments.emplace_back(new SegmentScene());
		//Always last
	segments.emplace_back(new SegmentAbout());

	//START
	for (int i = 0; i < segments.size(); ++i)
	{
		segments[i]->Start();
	}
	//
	ImGui_ImplSDL2_InitForOpenGL(WindowProperties::Instance()->window, App->renderer3D->GetGLContext());
	ImGui_ImplOpenGL3_Init();

	return true;
}

bool ModuleEditor::CleanUp()
{
ImGui_ImplOpenGL3_Shutdown();
ImGui_ImplSDL2_Shutdown();
ImGui::DestroyContext();

//Clean Segments
for (int i = 0; i < segments.size(); ++i)
{
	RELEASE(segments[i]);
}

//Delete Editor Properties
eProps->Delete();
//RELEASE(segAbout);
//RELEASE(segConfiguration);

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
	//ImGui::ShowDemoWindow();

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
#pragma endregion Imgui Render methods

#pragma region GUI
void ModuleEditor::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		//FILE
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit", "ALT+F4"))
			{
				App->StopEngine();
			}
			ImGui::EndMenu();
		}

		//VIEW
		if (ImGui::BeginMenu("View"))
		{

			for (int i = 0; i < (segments.size() - 1); ++i)
			{
				if (ImGui::MenuItem(segments[i]->name.c_str(), NULL, &segments[i]->enabled))
				{
					LOG("%s '%s'", segments[i]->name.c_str(), segments[i]->enabled ? "OPENED" : "CLOSED");
				}
			}
			ImGui::EndMenu();
		}

		//HELP
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Visit Github", ""))
			{
				ShellExecute(0, 0, "https://github.com/MrIllas/Lightsteam-Engine", 0, 0, SW_SHOW);
			}

			if (ImGui::MenuItem(segments[segments.size()-1]->name.c_str(), NULL, &segments[segments.size()-1]->enabled))
			{
				LOG("%s '%s'", segments[segments.size() - 1]->name.c_str(), segments[segments.size() - 1]->enabled ? "OPENED" : "CLOSED");
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ModuleEditor::UpdateSegments()
{
	for (int i = 0; i < segments.size(); ++i)
	{
		if (segments[i]->enabled)
		{
			segments[i]->Update();
		}
	}
}
#pragma endregion Gui Elements of the editor

#pragma region Save/Load Settings

void ModuleEditor::LoadSettingsData(pugi::xml_node& load)
{
	for (int i = 0; i < segments.size(); ++i)
	{
		segments[i]->enabled = load.child(segments[i]->name.c_str()).attribute("value").as_bool();
	}

	//Editor Properties
	eProps->colorMode = (COLORMODE) load.child("ColorMode").attribute("value").as_int();
	eProps->SwitchColorMode();
}

void ModuleEditor::SaveSettingsData(pugi::xml_node& save)
{
	for (int i = 0; i < segments.size(); ++i)
	{
		save.child(segments[i]->name.c_str()).attribute("value") = segments[i]->enabled;
	}

	save.child("ColorMode").attribute("value") = (int)eProps->colorMode;
}

#pragma endregion Save & Load of Settings