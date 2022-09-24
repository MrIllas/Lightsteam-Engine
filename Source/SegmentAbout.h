#pragma once
#include "Segment.h"

class SegmentAbout : public Segment
{
public:
	SegmentAbout(std::string name, bool enabled = false);
	~SegmentAbout();

	void Update() override;
};

