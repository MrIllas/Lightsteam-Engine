#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(std::string uuid) : Resource(uuid, RESOURCE_TYPE::TEXTURE)
{
	std::string aux = LIB_TEXTURES;
	libraryFile = aux;

	width = 0;
	height = 0;
	depth = 0;
	mips = 0;
	bytes = 0;
	gpu_id = 0;

	format = TEXTURE_FORMAT::UNKNOWN;
}

ResourceTexture::~ResourceTexture()
{
}

nlohmann::JsonData ResourceTexture::SaveUnique(nlohmann::JsonData data)
{
	

	return data;
}

//void ResourceTexture::Save(std::string& path) const
//{
//
//}
//
//void ResourceTexture::Load(const std::string& load)
//{
//
//}
