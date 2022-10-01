#include "SegmentScene.h"

SegmentScene::SegmentScene(bool enabled) : Segment(enabled)
{
	name = "Scene";

	frameBuffer = FrameBuffer::Instance();
}

SegmentScene::~SegmentScene()
{
	frameBuffer->Delete();
}

void SegmentScene::Start()
{
	//frameBuffer->CreateBuffer();
}

void SegmentScene::Update()
{
	if (ImGui::Begin(name.c_str()))
	{
		RenderSpace();
	}
	ImGui::End();
}

void SegmentScene::RenderSpace()
{
	ImGui::Image((ImTextureID)frameBuffer->GetTextureBuffer(), {1280, 720}, ImVec2(0, 1), ImVec2(1, 0));
}