#include "ResourceShader.h"

ResourceShader::ResourceShader(std::string uuid) : Resource(uuid, RESOURCE_TYPE::SHADER)
{

}

ResourceShader::~ResourceShader()
{
}

void ResourceShader::CleanInstance()
{
	if (shader != nullptr) RELEASE(shader);
}

nlohmann::JsonData ResourceShader::SaveUnique(nlohmann::JsonData data)
{
	data.SetInt("Binary Format", this->binaryFormat);
	return data;
}

void ResourceShader::LoadUnique(nlohmann::JsonData data)
{
	this->binaryFormat = data.GetInt("Binary Format");
}