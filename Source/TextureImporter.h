#pragma once

#include "Globals.h"

#include <iostream>
#include <vector>

#define LIB_TEXTURE "Library/Textures/"

typedef unsigned int uint;

struct Texture {
	unsigned int id;
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

	static Texture ImportTexture(std::string filePath);

	static void ImportToLibrary(ResourceTexture* uuid);

private:
	static int CheckTexturesLoaded(std::string filePath, Texture& texture);

	static void CheckerImage();

public:
	static Texture checkers;
private:
	static std::vector<TextureData*> texturesLoaded;

	
};

