#pragma once
#include "Resource.h"
class ResourceMesh : public Resource
{
public:
	ResourceMesh(std::string uuid);
	~ResourceMesh();

protected:
	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;
};

