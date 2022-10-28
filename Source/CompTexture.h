#pragma once
#include "Component.h"


class CompTexture : public Component
{
public:
	CompTexture(GameObject* owner);
	~CompTexture() override;

	void Update() override;

	void UpdateGUI() override;
};

