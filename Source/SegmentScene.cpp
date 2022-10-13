#include "SegmentScene.h"

#include "ModuleRenderer3D.h"
#include "FrameBuffer.h"

#include "MathGeoLib/include/Math/float2.h"
#include "ImGui/imgui_internal.h"

SegmentScene::SegmentScene(bool enabled) : Segment(enabled)
{
	name = "Scene";

	renInstance = RenderProperties::Instance();
}

SegmentScene::~SegmentScene()
{
	//renderer->CleanUp();
	//RELEASE(renderer);
}

void SegmentScene::Start()
{
}

void SegmentScene::Update()
{
	if (ImGui::Begin(name.c_str()))
	{
		ImVec2 aux = ImGui::GetWindowSize();
		aux.y -= 35;
		if (aux.x != segmentSize.x || aux.y != segmentSize.y)
		{
			segmentSize.x = aux.x;
			segmentSize.y = aux.y;
			if (renInstance->render == nullptr)
			{
				renInstance->render = new Renderer({ segmentSize.x, segmentSize.y });
				//render->render->Start();
			}
			else renInstance->render->Resize({ segmentSize.x, segmentSize.y });
		}

		RenderSpace();
	}
	ImGui::End();
}

void SegmentScene::RenderSpace()
{
	ImGui::Image((ImTextureID)renInstance->render->GetFrameBufffer()->GetTextureBuffer(), { segmentSize.x, segmentSize.y }, ImVec2(0, 1), ImVec2(1, 0));
	//renInstance->render->Draw();
}