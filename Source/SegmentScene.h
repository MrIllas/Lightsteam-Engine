#pragma once
#include "Segment.h"
#include "Renderer.h"

struct CameraProperties;
struct SceneProperties;
struct EditorProperties;
struct ResourceProperties;

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

	void DropTarget();

private:
	ImVec2 segmentSize;

	CameraProperties* camInstance = nullptr;
	SceneProperties* sceneInstance = nullptr;
	EditorProperties* editorInstance = nullptr;
	ResourceProperties* resourceInstance = nullptr;
};

