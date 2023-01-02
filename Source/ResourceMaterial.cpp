#include "ResourceMaterial.h"

#include "JsonUtils.h"

ResourceMaterial::ResourceMaterial(std::string uuid) : Resource(uuid, RESOURCE_TYPE::MATERIAL)
{

}

ResourceMaterial::~ResourceMaterial()
{

}

void ResourceMaterial::CleanInstance()
{
	if (material != nullptr)
	{
		LOG(LOG_TYPE::ATTENTION, "RC 0: Unloading material '%s' from memory!", libraryFile.c_str());
		RELEASE(material);
	}
}

void ResourceMaterial::PlanDelete()
{

}

void ResourceMaterial::ImportToLibrary()
{
	SetName(LibraryManager::GetFilename(assetsFile));
	this->material = new Material(name);
	this->material->uuid = uuid;

	std::string libDir = LIB_MATERIALS;
	libDir += "/" + uuid + ".material";
	//Save current material
	this->material->Save(libDir);
	this->material->Save(assetsFile);

	SetLibraryFile(libDir);

	//Unload Material
	RELEASE(this->material);
}

Material* ResourceMaterial::ImportFromLibrary()
{
	IncreaseRC();

	this->material = new Material("Material");
	nlohmann::JsonData data;

	try
	{
		char* buffer = nullptr;

		uint size = LibraryManager::Load(libraryFile, &buffer);
		data.data = nlohmann::ordered_json::parse(buffer, buffer + size);
		RELEASE(buffer);
	}
	catch (nlohmann::json::parse_error& ex)
	{
		LOG(LOG_TYPE::ERRO, "Error: Material parse at byte %i: %s", ex.byte, ex.what());
		return this->material;
	}

	this->material->Load(data);

	return this->material;
}

#pragma region Save&Load
nlohmann::JsonData ResourceMaterial::SaveUnique(nlohmann::JsonData data)
{

	return data;
}

void ResourceMaterial::LoadUnique(nlohmann::JsonData data)
{

}
#pragma endregion
