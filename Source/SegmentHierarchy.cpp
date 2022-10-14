#include "SegmentHierarchy.h"

#include "ModuleScene.h"

SegmentHierarchy::SegmentHierarchy(bool enabled) : Segment(enabled)
{
	name = "Hierarchy";
}

SegmentHierarchy::~SegmentHierarchy()
{

}

void SegmentHierarchy::Start()
{
	sceneInstance = SceneProperties::Instance();
}

void SegmentHierarchy::Update()
{
	if (ImGui::Begin(name.c_str()))
	{

	}
	ImGui::End();
}