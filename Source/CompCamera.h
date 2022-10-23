#pragma once
#include "Component.h"

#include "Camera.h"

class CompCamera : public Component
{
public:
	CompCamera(GameObject* owner);
	~CompCamera() override;

	void Init() override;
	void Update() override;

private:
	Camera camera;
};

