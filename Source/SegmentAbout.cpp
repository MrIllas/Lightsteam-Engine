#include "SegmentAbout.h"

#include "SDL/include/SDL.h"
#include "RapidJson/rapidjson.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

SegmentAbout::SegmentAbout(std::string name, bool enabled) : Segment(name, enabled)
{
	//Versions
	SDL_version version;
	SDL_GetVersion(&version);

	vSdl = "SDL " + std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch);
	vImGui = "DearImGui " + std::string(ImGui::GetVersion());
	vMathGeoLib = "MathGeoLib 1.5";
	vGlew = "Glew ";
	vGlew += (const char*)glewGetString(GLEW_VERSION);
	vOpenGl = "OpenGl ";
	vOpenGl += (const char*)glGetString(GL_VERSION);
	vRapidJson = "RapidJson " + std::to_string(RAPIDJSON_MAJOR_VERSION) + "." + std::to_string(RAPIDJSON_MINOR_VERSION) + "." + std::to_string(RAPIDJSON_PATCH_VERSION);

}

SegmentAbout::~SegmentAbout()
{

}

void SegmentAbout::Update()
{
	if (ImGui::Begin("About"))
	{
		ImGui::Text("Lightsteam Engine \n");
		ImGui::Spacing();
		ImGui::Text("A light 3D engine.");
		ImGui::Spacing();
		ImGui::TextURL("Made by Robert Recorda i Illas (MrIllas)", "https://github.com/MrIllas/Lightsteam-Engine");
		
		ImGui::NewLine();
		ThirdPartyLibs();

		ImGui::NewLine();
		MyLicense();	
	}
	ImGui::End();
}

#pragma region Text
void SegmentAbout::ThirdPartyLibs()
{
	ImGui::Text("3rd Party Libraries used: ");
	ImGui::BulletText(""); 
	ImGui::TextURL(vSdl.c_str(), "https://libsdl.org/index.php", 1, 0);

	ImGui::BulletText("");
	ImGui::TextURL(vImGui.c_str(), "https://github.com/ocornut/imgui", 1, 0);

	ImGui::BulletText("");
	ImGui::TextURL(vGlew.c_str(), "http://glew.sourceforge.net", 1, 0);

	ImGui::BulletText("");
	ImGui::TextURL(vOpenGl.c_str(), "https://www.opengl.org", 1, 0);

	ImGui::BulletText("");
	ImGui::TextURL(vRapidJson.c_str(), "https://github.com/Tencent/rapidjson", 1, 0);

	ImGui::BulletText("");
	ImGui::TextURL(vMathGeoLib.c_str(), "https://github.com/juj/MathGeoLib", 1, 0);
}

void SegmentAbout::MyLicense()
{
	ImGui::Text("License: ");
	ImGui::NewLine();
	ImGui::TextWrapped("MIT License");
	ImGui::NewLine();
	ImGui::TextWrapped("Copyright (c) 2022 MrIllas");
	ImGui::NewLine();
	ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the \Software\), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : ");
	ImGui::NewLine();
	ImGui::TextWrapped("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.");
	ImGui::NewLine();
	ImGui::TextWrapped("THE SOFTWARE IS PROVIDED \AS IS\, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
}
#pragma endregion About Text