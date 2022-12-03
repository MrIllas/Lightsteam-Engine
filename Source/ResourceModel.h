#pragma once
#include "Resource.h"


class MeshRenderer;

class ResourceModel : public Resource
{
public:
	ResourceModel(std::string uuid);
	~ResourceModel();

	//std::vector<std::string> meshesUuid;

	void CleanInstance() override;

	void CleanMeshRendererMap();

protected:
	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;
	void LoadUnique(nlohmann::JsonData data) override;
public:
	//For some reason this two maps require to be pointers or otherwise they leave memory leaks.
	std::map<std::string, MeshRenderer*>* meshRendererMap;
	std::map<std::string, std::string> *meshCCF = nullptr; //UUID, CFF 
};

