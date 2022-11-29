#include "Resource.h"

Resource::Resource(std::string uuid, RESOURCE_TYPE type)
{
	this->uuid = uuid;
	this->type = type;
}

Resource::~Resource()
{

}

RESOURCE_TYPE Resource::GetType() const
{
	return this->type;
}

std::string Resource::GetUUID() const
{
	return this->uuid;
}

bool Resource::IsLoadedToMemory() const
{

	return false;
}

bool Resource::LoadToMemory()
{

	return false;
}

nlohmann::JsonData Resource::SaveUnique(nlohmann::JsonData data)
{
	
	return data;
}

void Resource::Save(std::string savePath)
{
	nlohmann::JsonData data;

	data.SetString("Asset Filepath", this->assetsFile);
	data.SetString("Library Filepath", this->libraryFile);
	data.SetString("UUID", this->uuid);
	data.SetInt("Type", (int)this->type);

	LibraryManager::SaveJSON(savePath, SaveUnique(data).data.dump(4));
}

void Resource::Load(const std::string& path)
{

}