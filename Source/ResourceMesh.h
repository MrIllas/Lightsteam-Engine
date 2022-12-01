#pragma once
#include "Resource.h"
class ResourceMesh : public Resource
{
public:
	ResourceMesh(std::string uuid);
	~ResourceMesh();

	//std::vector<std::string> meshesUuid;

protected:
	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;
};

