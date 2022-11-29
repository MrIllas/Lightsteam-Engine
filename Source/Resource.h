#pragma once
#include "LibraryManager.h"


#include "JsonUtils.h"
enum class RESOURCE_TYPE
{
	TEXTURE,
	MESH,
	SCENE,
	UNKNOWN
};

class Resource
{
public:
	Resource(std::string uuid, RESOURCE_TYPE type);
	~Resource();

	RESOURCE_TYPE GetType() const;

	std::string GetUUID() const;

	std::string GetAssetsFile() const { return assetsFile; }
	void SetAssetsFile(std::string path) { assetsFile = path; }
	std::string GetLibraryFile() const { return libraryFile; }
	void SetLibraryFile(std::string path) { libraryFile = path; }

	bool IsLoadedToMemory() const;
	bool LoadToMemory();

	
	//virtual void LoadUnique(nlohmann::JsonData data);

	void Save(std::string savePath);
	void Load(const std::string& path);

protected:
	virtual nlohmann::JsonData SaveUnique(nlohmann::JsonData data);

protected:
	std::string uuid;
	std::string assetsFile;
	std::string libraryFile;
	
	RESOURCE_TYPE type = RESOURCE_TYPE::UNKNOWN;
	
public:
	uint referenceCount = 0;
};

