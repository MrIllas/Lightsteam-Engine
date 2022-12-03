#pragma once
#include "Resource.h"

struct TextureData;

enum class TEXTURE_FORMAT
{
	COLOR_INDEX,
	RGB,
	RGBA,
	BGR,
	BGRA,
	LUMINANCE,
	UNKNOWN
};

class ResourceTexture : public Resource
{
public:
	ResourceTexture(std::string uuid);
	~ResourceTexture();

	nlohmann::JsonData SaveUnique(nlohmann::JsonData data) override;

	void CleanInstance() override;
	/*bool IsLoadedToMemory() override;
	bool LoadToMemory();*/

	/*void Save(std::string& path) const override;
	void Load(const std::string& load) override;*/

public:
	TextureData* texture = nullptr;
};

