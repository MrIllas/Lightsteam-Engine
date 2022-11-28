#pragma once
#include "Globals.h"

#include <string>

class LibraryFolder;

namespace LibraryManager
{
	void Init(LibraryFolder& root);
	void CleanUp();

	void GenerateLibrary();

	bool Exists(std::string file);
	bool IsDirectory(std::string dir);
	bool CreateDir(std::string dir);
	std::string GetFilename(std::string file, bool extension = false);

	bool AddPath(std::string path); 
	std::string NormalizePath(std::string path);
	std::string UnNormalizePath(std::string path);

	unsigned int Load(std::string filePath, char** buffer);

	unsigned int Save(std::string filePath, char* data, uint size = 0, bool append = false);
	unsigned int Copy(std::string filePath, std::string dir, std::string& output);

	void SaveJSON(std::string filePath, std::string jsonDump);
	std::string LoadJSON(std::string filePath);

	void FolderUpdate(LibraryFolder* folder, bool recursive = false);
};