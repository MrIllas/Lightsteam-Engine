#pragma once

#include <iostream>

typedef unsigned int uint;

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	static uint CheckerImage();

	static uint ImportTexture(std::string filePath);
};

