#pragma once
#include "Segment.h"
class SegmentScene : public Segment
{
public:
	SegmentScene(bool enabled = false);
	~SegmentScene();

	void Update() override;
private:
	void RenderSpace();
};

