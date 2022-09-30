#include "SegmentScene.h"

SegmentScene::SegmentScene(bool enabled) : Segment(enabled)
{
	name = "Scene";


}

SegmentScene::~SegmentScene()
{

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

}