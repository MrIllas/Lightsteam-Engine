#pragma once

//#include "ImGui/imgui.h"
#include "ImGuiUtils.h"

#include <string>

class Segment
{
public:
	Segment(std::string name, bool enabled = false) 
	{
		this->name = name;
		this->enabled = enabled;
	};
	virtual ~Segment() {};

	virtual void Update(){};

public:

	bool enabled = false;

	std::string name = "EmptySegment";

};

