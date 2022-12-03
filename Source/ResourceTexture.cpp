#include "ResourceTexture.h"

#include "TextureImporter.h"

ResourceTexture::ResourceTexture(std::string uuid) : Resource(uuid, RESOURCE_TYPE::TEXTURE)
{
	/*std::string aux = LIB_TEXTURES;
	libraryFile = aux;*/
}

ResourceTexture::~ResourceTexture()
{
	
}

void ResourceTexture::CleanInstance()
{
	if (texture != nullptr)
	{
		LOG(LOG_TYPE::ATTENTION, "RC 0: Unloading texture '%s' from memory!", libraryFile.c_str());
		RELEASE(texture);
	}
}

nlohmann::JsonData ResourceTexture::SaveUnique(nlohmann::JsonData data)
{
	

	return data;
}


