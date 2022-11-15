#pragma once
#include "Component.h"

#include "MeshRenderer.h"

class CompTexture : public Component
{
public:
	CompTexture(GameObject* owner, std::string uuid);
	~CompTexture() override;

	void Init() override;
	void Update() override;

	void UpdateGUI() override;

	Texture GetTexture();

	void SetTexture(Texture texture);
	void SetTexture(unsigned int id, std::string path);

private:
	Texture texture;
	Texture checkersTexture;

	bool isCheckers;
};

