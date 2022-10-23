#pragma once
#include "Segment.h"

class GameObject;
struct SceneProperties;


class SegmentInspector : public Segment
{
public:
	SegmentInspector(bool enabled = false);
	~SegmentInspector() override;

	void Start() override;

	void Update() override;

private:
	GameObject* IterateGameObject(GameObject* go);

private:
	SceneProperties* sceneInstance = nullptr;
};

