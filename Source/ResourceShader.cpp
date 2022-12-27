#include "ResourceShader.h"

ResourceShader::ResourceShader(std::string uuid) : Resource(uuid, RESOURCE_TYPE::SHADER)
{

}

ResourceShader::~ResourceShader()
{

}

void ResourceShader::CleanInstance()
{
	
}

nlohmann::JsonData ResourceShader::SaveUnique(nlohmann::JsonData data)
{

	return data;
}