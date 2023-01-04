#pragma once
#include "LibraryManager.h"


#include "JsonUtils.h"
enum class RESOURCE_TYPE
{
	TEXTURE,
	MESH,
	MODEL,
	SCENE,
	SHADER,
	MATERIAL,
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

	virtual bool IsLoadedToMemory();
	virtual bool LoadToMemory();

	void IncreaseRC() { ++referenceCount; }
	void DecreaseRC() 
	{ 
		--referenceCount;

		if (referenceCount < 1) CleanInstance();
	}
	uint GetRC() { return referenceCount; }

	std::string GetName() { return name; }
	void SetName(std::string value) { this->name = value; }

	std::string GetVersion() { return version; }
	void SetVersion(std::string version) { this->version = version; }

	void Save();
	bool Load();

	//Called by resources when the reference count is 0. Unloads
	virtual void CleanInstance();
	virtual void PlanDelete() {}

protected:
	virtual nlohmann::JsonData SaveUnique(nlohmann::JsonData data);
	virtual void LoadUnique(nlohmann::JsonData data);
protected:
	std::string uuid;
	std::string name;
	std::string version;
	std::string assetsFile;
	std::string libraryFile;
	
	RESOURCE_TYPE type = RESOURCE_TYPE::UNKNOWN;
	
	uint referenceCount = 0;
};

