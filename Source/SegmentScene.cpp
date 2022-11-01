#include "SegmentScene.h"

#include "ModuleCamera3D.h"
#include "FrameBuffer.h"

#include "MathGeoLib/include/Math/float2.h"
#include "ImGui/imgui_internal.h"

SegmentScene::SegmentScene(bool enabled) : Segment(enabled)
{
	name = "Scene";

	camInstance = CameraProperties::Instance();
}

SegmentScene::~SegmentScene()
{
	//renderer->CleanUp();
	//RELEASE(renderer);
}

void SegmentScene::Start()
{
	//renInstance->render = new Renderer({ segmentSize.x, segmentSize.y });
	camInstance->editorCamera.renderer = new Renderer({ segmentSize.x, segmentSize.y });
}

void SegmentScene::Update()
{
	if (ImGui::Begin(name.c_str(), 0, ImGuiWindowFlags_NoScrollbar))
	{
		ImVec2 pos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		size.x += pos.x;
		size.y += pos.y;

		if (ImGui::IsMouseHoveringRect(pos, size)) camInstance->isMouseOnScene = true;
		else camInstance->isMouseOnScene = false;

		ImVec2 aux = ImGui::GetContentRegionAvail();
		aux.y = (aux.x / 16) * 9;


		if (aux.x != segmentSize.x || aux.y != segmentSize.y)
		{
			segmentSize.x = aux.x;
			segmentSize.y = aux.y;

			if (camInstance->editorCamera.renderer == nullptr)
			{
				camInstance->editorCamera.renderer = new Renderer({ segmentSize.x, segmentSize.y });
			}
			else camInstance->editorCamera.renderer->Resize({ segmentSize.x, segmentSize.y });

		}

		RenderSpace();
	}
	ImGui::End();
}

void SegmentScene::RenderSpace()
{
	float aux = (ImGui::GetWindowHeight()+20 - segmentSize.y) * 0.5f;


	ImGui::SetCursorPosY(aux);
	ImGui::Image((ImTextureID)camInstance->editorCamera.renderer->GetFrameBufffer()->GetTextureBuffer(), segmentSize, ImVec2(0, 1), ImVec2(1, 0));
}