#pragma once
#include "Resource.h"

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

	/*void Save(std::string& path) const override;
	void Load(const std::string& load) override;*/

public:
	uint width;
	uint height;

	uint depth;
	uint mips;

	uint bytes;

	uint gpu_id;

	TEXTURE_FORMAT format;

};

