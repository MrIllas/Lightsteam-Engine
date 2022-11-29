#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(std::string uuid) : Resource(uuid, RESOURCE_TYPE::MESH)
{

}

ResourceMesh::~ResourceMesh()
{

}

nlohmann::JsonData ResourceMesh::SaveUnique(nlohmann::JsonData data)
{
	return data;
}