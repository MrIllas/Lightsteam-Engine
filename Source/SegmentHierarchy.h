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
	GameObject* DisplayGameObject(GameObject* go);

	void RightClickMenuContextItem(GameObject* go = nullptr);
	void RightClickMenuContextWindow();
	void RightClickMenuContent(GameObject* go);

	void GetSelectedNode(GameObject* go);
	inline bool CleanSelected(GameObject* go);

private:
	SceneProperties* sceneInstance = nullptr;
	GameObject* hoverGO = nullptr;
	GameObject* dropGO = nullptr;


	bool cleanSelected = false;
	bool winHover = false;
};

