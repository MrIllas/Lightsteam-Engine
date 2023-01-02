#pragma once
#include "Resource.h"

#include "Material.h"

class ResourceMaterial : public Resource
{
public:
	ResourceMaterial(std::string uuid);
	~ResourceMaterial();

	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;
	void LoadUnique(nlohmann::JsonData data) override;

	void CleanInstance() override;
	void PlanDelete() override;

	void ImportToLibrary();
	Material* ImportFromLibrary();

public:
	Material* material = nullptr;

};

