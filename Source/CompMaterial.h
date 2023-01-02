#pragma once
#include "Component.h"

class Material;
class Shader;
struct ResourceProperties;

class CompMaterial : public Component
{
public:
	CompMaterial(GameObject* owner, std::string uuid);
	~CompMaterial() override;

	void Init() override;
	void Update() override;

	void UpdateGUI() override;
	

	nlohmann::ordered_json SaveUnique(nlohmann::JsonData data) override;
	void LoadUnique(nlohmann::JsonData data) override;

private:
	void UpdateDragDrop();
	void ShaderSelectorCombo();
	void ShaderCustomGUI();

	ResourceProperties* resInstance = nullptr;

public:
	Material* material = nullptr;
};

