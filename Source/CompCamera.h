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

	nlohmann::ordered_json SaveUnique(nlohmann::JsonData data) override;
	void LoadUnique(nlohmann::JsonData data) override;

	void Render();

public:
	Camera camera;

	bool isMainCamera;

private:
	CameraProperties* camInstance = nullptr;
};

