#pragma once
#include "Component.h"

#include "Camera.h"

class CameraProperties;

class CompCamera : public Component
{
public:
	CompCamera(GameObject* owner, std::string uuid);
	~CompCamera() override;

	void Init() override;
	void Update() override;
	
	void UpdateGUI() override;

public:
	Camera camera;

	bool isMainCamera;

private:
	CameraProperties* camInstance = nullptr;
};

