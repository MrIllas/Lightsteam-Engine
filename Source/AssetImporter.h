#pragma once

#include <string>

class AssetImporter
{
public:
	AssetImporter();
	~AssetImporter();

	void Start();
	void CleanUp();

public:
	void LoadFile( std::string filePath);
};



