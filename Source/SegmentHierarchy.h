#pragma once
#include "Segment.h"

struct SceneProperties;
class GameObject;

class SegmentHierarchy : public Segment
{
public:
	SegmentHierarchy(bool enabled = false);
	~SegmentHierarchy() override;

	void Start() override;

	void Update() override;

private:
	void DisplayGameObject(GameObject* go);

	void RightClickMenu();

	void GetSelectedNode(GameObject* go);
	inline bool CleanSelected(GameObject* go);

private:
	SceneProperties* sceneInstance = nullptr;

	bool cleanSelected = false;
};

