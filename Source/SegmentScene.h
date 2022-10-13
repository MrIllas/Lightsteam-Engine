#pragma once
#include "Segment.h"
#include "Renderer.h"

struct RenderProperties;

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
	ImVec2 segmentSize = { 0, 0 };
	//Renderer* renderer = nullptr;

	RenderProperties* renInstance = nullptr;
};

