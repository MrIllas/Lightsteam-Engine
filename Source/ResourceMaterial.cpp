#include "ResourceMaterial.h"

#include "JsonUtils.h"
#include "CompMaterial.h"
#include "Material.h"

ResourceMaterial::ResourceMaterial(std::string uuid) : Resource(uuid, RESOURCE_TYPE::MATERIAL)
{
	compRef = new std::vector<CompMaterial*>();
}

ResourceMaterial::~ResourceMaterial()
{

}

void ResourceMaterial::CleanInstance()
{
	if (compRef != nullptr)
	{
		compRef->clear();
		RELEASE(compRef);
	}

	if (material != nullptr)
	{
		LOG(LOG_TYPE::ATTENTION, "RC 0: Unloading material '%s' from memory!", libraryFile.c_str());
		RELEASE(material);
	}
}

void ResourceMaterial::PlanDelete()
{
	for (int i = 0; i < compRef->size(); ++i)
	{
		compRef->at(i)->material = nullptr;
		compRef->at(i) = nullptr;
	}
	compRef->clear();
}

void ResourceMaterial::ImportToLibrary(Material* material)
{
	SetName(LibraryManager::GetFilename(assetsFile));
	if (material != nullptr)
	{
		this->material = material;
		this->material->uuid = uuid;
	}
	else
	{
		this->material = new Material(name);
		this->material->uuid = uuid;
		this->material->SetDefaultShader(nullptr);
	}

	std::string libDir = LIB_MATERIALS;
	libDir += "/" + uuid + ".material";
	//Save current material
	this->material->Save(libDir);
	this->material->Save(assetsFile);

	SetLibraryFile(libDir);

	//Unload Material
	RELEASE(this->material);
}

void ResourceMaterial::ImportFromLibrary(CompMaterial* comp)
{
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
		comp->material = nullptr;
	}

	this->material->Load(data);
	
	SetMaterialToComp(comp);
}

void ResourceMaterial::SetMaterialToComp(CompMaterial* comp)
{
	comp->material = this->material;
	IncreaseRC();
	compRef->push_back(comp);
}

void ResourceMaterial::RemoveMaterialToComp(CompMaterial* comp)
{
	auto it = std::find(compRef->begin(), compRef->end(), comp);
	
	if (it != compRef->end())
	{
		compRef->erase(it);
	}

	DecreaseRC();
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
