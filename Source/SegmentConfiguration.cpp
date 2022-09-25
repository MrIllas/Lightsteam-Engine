#include "SegmentConfiguration.h"

SegmentConfiguration::SegmentConfiguration(std::string name, bool enabled) : Segment(name, enabled)
{
	wProps = WindowProperties::Instance();
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
	}
	ImGui::End();
}

#pragma region Configurations
void SegmentConfiguration::ApplicationHeader()
{

}

void SegmentConfiguration::WindowHeader()
{
	float f = 800;
	ImGui::SliderFloat("Brightness", &wProps->brightness, 0.f, 1.0f);
	ImGui::Text("Window Size -> ( %i, %i )", wProps->w, wProps->h);

	bool vsync = false;
	ImGui::Checkbox("Vsync ", &vsync);
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
#pragma endregion Configuration methods