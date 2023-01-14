#pragma once
#include "Resource.h"

class Material;
class CompMaterial;

class ResourceMaterial : public Resource
{
public:
	ResourceMaterial(std::string uuid);
	~ResourceMaterial();

	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;
	void LoadUnique(nlohmann::JsonData data) override;

	void CleanInstance() override;
	void PlanDelete() override;

	void ImportToLibrary(Material* material = nullptr);
	void ImportFromLibrary(CompMaterial* comp);

	Material* GetMaterial() { return material; }
	void SetMaterialToComp(CompMaterial* comp);
	void RemoveMaterialToComp(CompMaterial* comp);

private:
	Material* material = nullptr;
	std::vector<CompMaterial*>* compRef = nullptr;
};

