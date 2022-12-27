#pragma once
#include "Resource.h"

#include "Shader.h"

class ResourceShader : public Resource
{
public:
	ResourceShader(std::string uuid);
	~ResourceShader();

	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;

	void CleanInstance() override; 

public:
	Shader* shader = nullptr;
};

