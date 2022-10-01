#pragma once
#include "Segment.h"
#include "FrameBuffer.h"

class SegmentScene : public Segment
{
public:
	SegmentScene(bool enabled = false);
	~SegmentScene();

	void Update() override;

	void Start() override;

private:
	void RenderSpace();

private:
	FrameBuffer* frameBuffer = nullptr;
};

