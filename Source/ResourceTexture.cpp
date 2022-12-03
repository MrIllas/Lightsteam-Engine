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
		RELEASE(texture);
	}
}

nlohmann::JsonData ResourceTexture::SaveUnique(nlohmann::JsonData data)
{
	

	return data;
}


