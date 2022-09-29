#include "SegmentConfiguration.h"

#include "Application.h"

#include "Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "SDL/include/SDL.h"

SegmentConfiguration::SegmentConfiguration(bool enabled) : Segment(enabled)
{
	name = "Configuration";

	wProps = WindowProperties::Instance();
	rProps = RenderProperties::Instance();
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
}

void SegmentConfiguration::WindowHeader()
{
	ImGui::SliderFloat("Brightness", &wProps->brightness, 0.f, 1.0f);
	ImGui::Text("Window Size -> ( %i, %i )", wProps->w, wProps->h);

	if (ImGui::Checkbox("Fullscreen", &wProps->fullscreen))
	{
		wProps->ToggleFullscreen();
	}
	if (ImGui::Checkbox("Fullscreen Desktop", &wProps->fullScreenDesktop))
	{
		wProps->ToggleFullscreenDesktop();
	}
	if (ImGui::Checkbox("Borderless", &wProps->borderless))
	{
		wProps->ToggleBorderless();
	}
	if (ImGui::Checkbox("Resizable", &wProps->resizable))
	{
		wProps->ToggleResizable();
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
#pragma endregion Configuration methods

void SegmentConfiguration::GetCaps()
{

}