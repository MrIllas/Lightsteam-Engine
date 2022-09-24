#include "SegmentAbout.h"


SegmentAbout::SegmentAbout(std::string name, bool enabled)
{
	this->name = name;
	this->enabled = enabled;
}

SegmentAbout::~SegmentAbout()
{

}

void SegmentAbout::Update()
{
	if (ImGui::Begin("About"))
	{

		ImGui::End();
	}

}