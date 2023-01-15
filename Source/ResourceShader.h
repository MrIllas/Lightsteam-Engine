#pragma once
#include "Resource.h"

#include "Shader.h"

class Material;

class ResourceShader : public Resource
{
public:
	ResourceShader(std::string uuid);
	~ResourceShader();

	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;
	void LoadUnique(nlohmann::JsonData data) override;

	void CleanInstance() override; 
	void PlanDelete() override;
	
	void SetShader(Shader* shader);
	Shader* GetShader() { return shader; }

	void SetShaderToMat(Material* mat);
	void RemoveShaderFromMat(Material* mat);

public:
	uint binaryFormat = 0;
	bool isOpenOnEditor = false;

private:
	Shader* shader = nullptr;
	std::vector<Material*>* matRef = nullptr;
};

