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

public:
	TextureData* texture = nullptr;
};

