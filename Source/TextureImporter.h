#pragma once

#include "Globals.h"

#include <iostream>
#include <vector>

#define LIB_TEXTURE "Library/Textures/"

typedef unsigned int uint;

struct Texture {
	unsigned int id;
	std::string resUuid;
	std::string path;
	uint w, h;
};

struct TextureData
{
	TextureData()
	{
	}
	Texture texture;
	BYTE* data = nullptr;
};

class ResourceTexture;

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	static TextureData* ImportTexture(std::string filePath);

	static Texture ImportFromLibrary(ResourceTexture* resource);
	static void ImportToLibrary(ResourceTexture* resource);

	static void CheckerImage();

public:
	static Texture checkers;

	
};

