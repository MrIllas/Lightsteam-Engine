#include "ModuleEditor.h"

#include "Application.h"
#include "ModuleWindow.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

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

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetSDLWindow(), App->window->GetGLContext());
	ImGui_ImplOpenGL3_Init();

	return true;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

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
}

void ModuleEditor::EndRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
#pragma endregion Editor Render methods

#pragma region GUI
UpdateStatus ModuleEditor::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Fullscreen"))
			{
				App->window->ToggleFullScreen();
			}

			if (ImGui::MenuItem("Exit", "ALT+F4"))
			{
				App->StopEngine();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	return UPDATE_CONTINUE;
}
#pragma endregion Gui Elements of the editor