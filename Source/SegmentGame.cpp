#include "SegmentGame.h"

#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Renderer.h"
#include "FrameBuffer.h"

SegmentGame::SegmentGame(bool enabled) : Segment(enabled)
{
	name = "Game";

	segmentSize = { 0, 0 };


}

SegmentGame::~SegmentGame()
{

}

void SegmentGame::Start()
{
	camInstance = CameraProperties::Instance();
}

void SegmentGame::Update()
{
	if (mainCamera == nullptr) LookForCamera();


	if (ImGui::Begin(name.c_str(), 0, ImGuiWindowFlags_NoScrollbar))
	{
		if (mainCamera != nullptr)
		{
			ImVec2 pos = ImGui::GetWindowPos();
			ImVec2 size = ImGui::GetWindowSize();
			size.x += pos.x;
			size.y += pos.y;

			ImVec2 aux = ImGui::GetContentRegionAvail();
			aux.y = (aux.x / 16) * 9;


			if (aux.x != segmentSize.x || aux.y != segmentSize.y)
			{
				segmentSize.x = aux.x;
				segmentSize.y = aux.y;

				if (mainCamera->camera.renderer == nullptr)
				{
					mainCamera->camera.SetRenderer({ segmentSize.x, segmentSize.y });
					//mainCamera->camera.renderer = new Renderer({ segmentSize.x, segmentSize.y });
				}
				else mainCamera->camera.renderer->Resize({ segmentSize.x, segmentSize.y });

			}

			
		}
		RenderSpace();
	}
	ImGui::End();
}

void SegmentGame::LookForCamera()
{
	if (camInstance->gameCameras.size() != 0)
		mainCamera = camInstance->gameCameras.at(camInstance->mainCameraId);
}

void SegmentGame::RenderSpace()
{
	if (mainCamera != nullptr)
	{
		float aux = (ImGui::GetWindowHeight() + 20 - segmentSize.y) * 0.5f;
		ImGui::SetCursorPosY(aux);
		ImTextureID texID = (ImTextureID) camInstance->gameCameras.at(camInstance->mainCameraId)->camera.renderer->GetFrameBufffer()->GetTextureBuffer();
		ImGui::Image(texID, segmentSize, ImVec2(0, 1), ImVec2(1, 0));
	}	
	else
	{
		std::string text = "No Main Camera set!";
		float font_size = ImGui::GetFontSize() * text.size() / 2;
		ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));

		ImGui::SetWindowFontScale(3);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y / 2);
		ImGui::Text(text.c_str());
	}
		
}