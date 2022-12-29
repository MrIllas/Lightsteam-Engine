#pragma once
#include "Resource.h"

#include "Shader.h"

class ResourceShader : public Resource
{
public:
	ResourceShader(std::string uuid);
	~ResourceShader();

	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;
	void LoadUnique(nlohmann::JsonData data) override;

	void CleanInstance() override; 

	void PlanDelete() override;

public:
	Shader* shader = nullptr;
	
	uint binaryFormat = 0;

	bool isOpenOnEditor = false;
};

