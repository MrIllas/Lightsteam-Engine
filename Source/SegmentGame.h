#pragma once
#include "Segment.h"

struct CameraProperties;
class CompCamera;

class SegmentGame : public Segment
{
public:
	SegmentGame(bool enabled = false);
	~SegmentGame();

	void Start() override;

	void Update() override;

private:
	void Resize();
	void RenderSpace();

	void LookForCamera();

private:
	ImVec2 segmentSize;

	CameraProperties* camInstance = nullptr;
	
	CompCamera* mainCamera = nullptr;
	int cameraID;
};

