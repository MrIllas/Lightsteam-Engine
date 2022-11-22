#pragma once
#include "Segment.h"
#include "Renderer.h"

struct CameraProperties;
struct SceneProperties;

class Camera;
class GameObject;

class SegmentScene : public Segment
{
public:
	SegmentScene(bool enabled = false);
	~SegmentScene();

	void Update() override;

	void Start() override;

private:
	void RenderSpace();
	void Guizmo(Camera& cam, GameObject* go);

private:
	ImVec2 segmentSize;

	CameraProperties* camInstance = nullptr;
	SceneProperties* sceneInstance = nullptr;
};

