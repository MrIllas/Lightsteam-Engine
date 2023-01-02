#pragma once
#include "Resource.h"


class MeshRenderer;


struct SubMeshResource
{
	MeshRenderer* meshRenderer = nullptr;
	uint referenceCount = 0;
	std::string libPath;

	void DecreaseRC()
	{
		--referenceCount;
		if (referenceCount <= 0)
		{
			LOG(LOG_TYPE::ATTENTION, "RC 0: Unloading mesh '%s' from memory!", libPath.c_str());
			RELEASE(meshRenderer);
			meshRenderer = nullptr;
		}
	}

};

class ResourceModel : public Resource
{
public:
	ResourceModel(std::string uuid);
	~ResourceModel();

	void CleanInstance() override;
	void PlanDelete() override;

	void CleanMeshRendererMap();
	
protected:
	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;
	void LoadUnique(nlohmann::JsonData data) override;
public:
	//For some reason this two maps require to be pointers or otherwise they leave memory leaks.
	std::map<std::string, SubMeshResource*>* meshRendererMap;
};

