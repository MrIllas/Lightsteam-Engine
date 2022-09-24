#pragma once

#include "ImGui/imgui.h"

#include <string>

class Segment
{
public:
	Segment() {};
	virtual ~Segment() {};

	virtual void Update(){};

public:

	bool enabled = false;

	std::string name = "EmptySegment";

};

