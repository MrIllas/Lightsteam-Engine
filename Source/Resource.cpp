#include "Resource.h"

Resource::Resource(std::string uuid, RESOURCE_TYPE type)
{
	this->uuid = uuid;
	this->type = type;
}

Resource::~Resource()
{
	CleanInstance();
}

RESOURCE_TYPE Resource::GetType() const
{
	return this->type;
}

std::string Resource::GetUUID() const
{
	return this->uuid;
}

bool Resource::IsLoadedToMemory()
{

	return false;
}

bool Resource::LoadToMemory()
{

	return false;
}

void Resource::CleanInstance()
{

}

nlohmann::JsonData Resource::SaveUnique(nlohmann::JsonData data)
{
	
	return data;
}

void Resource::LoadUnique(nlohmann::JsonData data)
{

}

void Resource::Save()
{
	nlohmann::JsonData data;
	std::string savePath = assetsFile;
	savePath += ".meta";

	data.SetString("Asset Filepath", this->assetsFile);
	data.SetString("Library Filepath", this->libraryFile);
	data.SetString("UUID", this->uuid);
	data.SetInt("Type", (int)this->type);

	LibraryManager::SaveJSON(savePath, SaveUnique(data).data.dump(4));
}

/// <summary>
/// Returns false if the file doesn't exists.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
bool Resource::Load()
{
	std::string metaPath = assetsFile;
	metaPath += ".meta";

	nlohmann::JsonData data;

	char* buffer = nullptr;
	uint size = LibraryManager::Load(metaPath, &buffer);

	if (buffer == nullptr) return false;

	data.data = nlohmann::ordered_json::parse(buffer, buffer + size);

	//Load vars
	assetsFile = data.GetString("Asset Filepath");
	libraryFile = data.GetString("Library Filepath");
	uuid = data.GetString("UUID");
	type = (RESOURCE_TYPE) data.GetInt("Type");

	LoadUnique(data);

	RELEASE(buffer);

	return true;
}