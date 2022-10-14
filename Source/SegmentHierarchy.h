#pragma once
#include "Segment.h"

struct SceneProperties;

class SegmentHierarchy : public Segment
{
public:
	SegmentHierarchy(bool enabled = false);
	~SegmentHierarchy();

	void Start() override;

	void Update() override;
private:
	SceneProperties* sceneInstance = nullptr;
};

