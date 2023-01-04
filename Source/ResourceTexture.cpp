#include "ResourceTexture.h"

#include "TextureImporter.h"

#include "Glew/include/glew.h"

ResourceTexture::ResourceTexture(std::string uuid) : Resource(uuid, RESOURCE_TYPE::TEXTURE)
{
}

ResourceTexture::~ResourceTexture()
{
	
}

void ResourceTexture::CleanInstance()
{
	if (texture != nullptr)
	{
		LOG(LOG_TYPE::ATTENTION, "RC 0: Unloading texture '%s' from memory!", libraryFile.c_str());
		
		const GLuint* id = new GLuint(texture->texture.id);
		glDeleteTextures(1, id);
		RELEASE(texture);
		RELEASE(id);
	}
}

nlohmann::JsonData ResourceTexture::SaveUnique(nlohmann::JsonData data)
{
	

	return data;
}


