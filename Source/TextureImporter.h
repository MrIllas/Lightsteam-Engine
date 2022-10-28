#pragma once

#include "Globals.h"

#include <iostream>
#include <vector>

typedef unsigned int uint;

struct TextureData
{
	TextureData()
	{
		id = 0;
		data = nullptr;
		path = "";
	}
	uint id;
	BYTE* data = nullptr;
	std::string path;
};

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	

	static uint ImportTexture(std::string filePath);
private:
	static int CheckTexturesLoaded(std::string filePath);

	static void CheckerImage();
public:
	static uint checkersID;
private:
	static std::vector<TextureData*> texturesLoaded;

	
};

