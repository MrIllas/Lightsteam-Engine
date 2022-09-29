#include "SegmentConfiguration.h"

#include "Application.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"

#include "Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "SDL/include/SDL.h"

SegmentConfiguration::SegmentConfiguration(bool enabled) : Segment(enabled)
{
	name = "Configuration";

	wProps = WindowProperties::Instance();
	rProps = RenderProperties::Instance();
	eProps = EditorProperties::Instance();
	time = Time::Instance();

	//GetCaps();
	vsync = rProps->vsync;

	//framerate queue
	for (int i = 0; i < 99; i++)
	{
		framerateQueue.emplace_back(0);
	}
	//miliseconds queue
	for (int i = 0; i < 99; i++)
	{
		milisecondsQueue.emplace_back(0);
	}
}

SegmentConfiguration::~SegmentConfiguration()
{

}

void SegmentConfiguration::Update()
{
	io = ImGui::GetIO(); (void)io;
	if (ImGui::Begin("Configuration"))
	{
		if (ImGui::CollapsingHeader("Application")) ApplicationHeader();
		if (ImGui::CollapsingHeader("Window")) WindowHeader();
		if (ImGui::CollapsingHeader("Input")) InputHeader();
		if (ImGui::CollapsingHeader("Rendering")) RenderingHeader();
		if (ImGui::CollapsingHeader("Editor")) EditorHeader();
	}
	ImGui::End();
}

#pragma region Configurations
void SegmentConfiguration::ApplicationHeader()
{
	//Framerate
	framerateQueue.erase(framerateQueue.begin());
	framerateQueue.emplace_back(io.Framerate);
	std::string auxTxt = "Framerate: " + std::to_string((int)framerateQueue.back());
	ImGui::PlotHistogram("", &framerateQueue[0], framerateQueue.size(), 0, auxTxt.c_str(), 0, 180, ImVec2(0, 160.0f));
	
	//Miliseconds
	milisecondsQueue.erase(milisecondsQueue.begin());
	milisecondsQueue.emplace_back(io.DeltaTime * 1000);
	auxTxt = "Miliseconds: " + std::to_string((int)milisecondsQueue.back());
	ImGui::PlotHistogram("", &milisecondsQueue[0], milisecondsQueue.size(), 0, auxTxt.c_str(), 0, 100, ImVec2(0, 160.0f));
	
	//Cap Fps
	ImGui::SliderInt("Cap FPS", &time->frameCap, 1, 180);
	if (ImGui::IsItemDeactivatedAfterEdit()) LOG("Fps capped to '%i'", time->frameCap);

	//if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	//{
	//	LOG("Hey");
	//}
}

void SegmentConfiguration::WindowHeader()
{
	ImGui::SliderFloat("Brightness", &wProps->brightness, 0.f, 1.0f);
	if (ImGui::IsItemDeactivatedAfterEdit()) LOG("Brightness set to '%.2f'", wProps->brightness);

	ImGui::Text("Window Size -> ( %i, %i )", wProps->w, wProps->h);

	if (ImGui::Checkbox("Fullscreen", &wProps->fullscreen))
	{
		wProps->ToggleFullscreen();
		LOG("Fullscreen '%s'", wProps->fullscreen ? "ON" : "OFF");

	}
	if (ImGui::Checkbox("Fullscreen Desktop", &wProps->fullScreenDesktop))
	{
		wProps->ToggleFullscreenDesktop();
		LOG("Fullscreen Desktop '%s'", wProps->fullScreenDesktop ? "ON" : "OFF");
	}
	if (ImGui::Checkbox("Borderless", &wProps->borderless))
	{
		wProps->ToggleBorderless();
		LOG("Borderless '%s'", wProps->borderless ? "ON" : "OFF");
	}
	if (ImGui::Checkbox("Resizable", &wProps->resizable))
	{
		wProps->ToggleResizable();
		LOG("Resizable '%s'", wProps->resizable ? "ON" : "OFF");
	}
	//ImGui::togg
}

void SegmentConfiguration::InputHeader()
{
	if (ImGui::IsMousePosValid())
	{
		ImGui::Text("Mouse position: (%g, %g)", io.MousePos.x, io.MousePos.y);
		ImGui::Spacing();
		ImGui::Text("Mouse motion: (%g,  %g)", io.MouseDelta.x, io.MouseDelta.y);
		ImGui::Spacing();
		ImGui::Text("Mouse Wheel: %g", io.MouseWheel);
		ImGui::Spacing();
		ImGui::Text("Key: "); //ImGui::Text("Chars queue:");
		for (int i = 0; i < io.InputQueueCharacters.Size; i++) 
		{ 
			ImWchar c = io.InputQueueCharacters[i]; 
			ImGui::SameLine();  
			ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); 
		}
	}
}

void SegmentConfiguration::RenderingHeader()
{
	if (ImGui::Checkbox("Vsync ", &rProps->vsync))
	{
		rProps->ToggleVsync(); 
		LOG("Vsync '%s'", rProps->vsync ? "ON" : "OFF");
	}

	if (ImGui::Checkbox("DepthTest", &rProps->depthTest))
	{
		rProps->ToggleDepthTest();
		LOG("Depth Test '%s'", rProps->depthTest ? "ON" : "OFF");
	}

	if (ImGui::Checkbox("Cull Face", &rProps->cullFace))
	{
		rProps->ToggleCullFace();
		LOG("Cull Face '%s'", rProps->cullFace ? "ON" : "OFF");
	}

	if (ImGui::Checkbox("Lighting", &rProps->lighting))
	{
		rProps->ToggleLighting();
		LOG("Lighting '%s'", rProps->lighting ? "ON" : "OFF");
	}

	if (ImGui::Checkbox("Color Material", &rProps->colorMaterial))
	{
		rProps->ToggleColorMaterial();
		LOG("Color Material '%s'", rProps->colorMaterial ? "ON" : "OFF");
	}

	if (ImGui::Checkbox("Texture 2D", &rProps->texture2D))
	{
		rProps->ToggleTexture2D();
		LOG("Texture 2D '%s'", rProps->texture2D ? "ON" : "OFF");
	}

	ImGui::Separator();
	ImGui::Text("CPU cores: %i ", SDL_GetCPUCount());
	ImGui::Text("System RAM: %.1f Gb", (SDL_GetSystemRAM() / 1000.0f));
	/*ImGui::Text("Caps: %s", strCaps);*/
	ImGui::Separator();

	ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
	ImGui::Text("GPU: %s", glGetString(GL_RENDERER));

	ImGui::Text("VRAM Budget: ");
	ImGui::Text("VRAM Usage: ");
	ImGui::Text("VRAM Available: ");
	ImGui::Text("VRAM Reserved: ");
}

void SegmentConfiguration::EditorHeader()
{
	/*if (ImGui::Checkbox("Light mode ", &eProps->lightMode))
	{
		eProps->ToggleLightMode();
		LOG("Light mode '%s'", eProps->lightMode ? "ON" : "OFF");
	}*/
	int aux = (int)&eProps->colorMode;
	if (ImGui::RadioButton("Light mode", (eProps->colorMode == COLORMODE::LightMode ? true : false)))
	{
		eProps->colorMode = COLORMODE::LightMode;
		eProps->SwitchColorMode();
		LOG("Light mode 'ON'");
	}
	if (ImGui::RadioButton("Dark mode", (eProps->colorMode == COLORMODE::DarkMode ? true : false)))
	{
		eProps->colorMode = COLORMODE::DarkMode;
		eProps->SwitchColorMode();
		LOG("Dark mode 'ON'");
	}
	if (ImGui::RadioButton("Classic mode", (eProps->colorMode == COLORMODE::ClassicMode ? true : false)))
	{
		eProps->colorMode = COLORMODE::ClassicMode;
		eProps->SwitchColorMode();
		LOG("Classic mode 'ON'");
	}
}
#pragma endregion Configuration methods

void SegmentConfiguration::GetCaps()
{

}