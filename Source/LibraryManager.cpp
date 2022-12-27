#include "LibraryManager.h"

#include "Globals.h"

#include "LibraryFolder.h"

#include "SDL/include/SDL.h"
#include "PhysFS/include/physfs.h"

void LibraryManager::Init(LibraryFolder& root)
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	SDL_free(base_path);


	if (PHYSFS_setWriteDir(".") == 0)
		LOG(LOG_TYPE::NONE, "File System error while creating write dir: %s\n", PHYSFS_getLastError());

	AddPath(".");
	AddPath("Assets");

	GenerateLibrary();
	FolderSystemUpdate(&root, true);
}

void LibraryManager::CleanUp()
{
	PHYSFS_deinit();
}

void LibraryManager::GenerateLibrary()
{
	std::string aux = LIB_ROOT;
	CreateDir(aux);

	aux = LIB_MESHES;
	CreateDir(aux);

	aux = LIB_MATERIALS;
	CreateDir(aux);

	aux = LIB_MODELS;
	CreateDir(aux);

	aux = LIB_TEXTURES;
	CreateDir(aux);

	aux = LIB_SCENES;
	CreateDir(aux);

	aux = LIB_SHADERS;
	CreateDir(aux);
}

void LibraryManager::FolderSystemUpdate(LibraryFolder* folder, bool recursive)
{
	//if (folder->children.size() != 0 || folder->libItem.size() != 0) folder->CleanUp();
	//else return; //Nothing to update - Return

	char** aux = PHYSFS_enumerateFiles(folder->path.c_str());

	std::string extension;

	for (char* c = *aux; c; c = *++aux) {
		std::string path = folder->path;
		path += "/";
		path += c;
		

		if (folder->ContainsPath(path)) continue; //Continue if the path already exists


		if (IsDirectory(path))
		{
			LibraryFolder* dir = new LibraryFolder(path, c, folder);
			folder->children.emplace_back(dir);
			if (recursive) FolderSystemUpdate(dir);
		}
		else
		{
			//Get Extension
			size_t pos = path.find_last_of(".");
			if (pos != std::string::npos)
				extension = path.substr(pos + 1);

			if (extension != "meta") //Ignore if extension is meta
			{
				std::string meta = path;
				meta += ".meta";

				folder->libItem.emplace_back(new LibraryItem(path, c, extension, Exists(meta)));
			}
		}
	}
}

bool LibraryManager::Exists(std::string file)
{
	return PHYSFS_exists(file.c_str()) != 0;
}

bool LibraryManager::IsDirectory(std::string dir)
{
	return PHYSFS_isDirectory(dir.c_str()) != 0;
}

bool LibraryManager::CreateDir(std::string dir)
{
	if (!IsDirectory(dir))
	{
		PHYSFS_mkdir(dir.c_str());
		return true;
	}
	return false;
}

bool LibraryManager::RemoveFile(std::string filePath)
{
	if (!Exists(filePath))
	{
		LOG(LOG_TYPE::ERRO, "ERROR: Couldn't delete '%s', file not found.", filePath.c_str());
		return false;
	}

	int num = PHYSFS_delete(filePath.c_str());
	LOG(LOG_TYPE::SUCCESS, "SUCCESS: File '%s' deleted. %i", filePath.c_str(), num);
}

std::string LibraryManager::GetFilename(std::string file, bool extension)
{
	std::string toReturn = file;

	size_t pos = 0;

	pos = toReturn.find_last_of("//");
	if (pos != std::string::npos) toReturn = toReturn.substr(pos + 1);

	pos = toReturn.find_last_of("\\");
	if (pos != std::string::npos) toReturn = toReturn.substr(pos + 1);

	if (!extension)
	{
		pos = toReturn.find_last_of(".");
		if (pos != std::string::npos) toReturn = toReturn.erase(pos);
	}

	return toReturn;
}

bool LibraryManager::AddPath(std::string path)
{
	if (PHYSFS_mount(path.c_str(), nullptr, 1) == 0)
	{
		LOG(LOG_TYPE::ERRO, "File System error while adding a path: %s", PHYSFS_getLastError());
	}
	else return true;

	return false;
}

std::string LibraryManager::NormalizePath(std::string path)
{
	for (int i = 0; i < path.size(); ++i)
	{
		if (path[i] == '\\')
			path[i] = '/';
	}
	return path;
}

std::string LibraryManager::UnNormalizePath(std::string path)
{
	for (int i = 0; i < path.size(); ++i)
	{
		if (path[i] == '/')
			path[i] = '\\';
	}
	return path;
}

uint LibraryManager::Load(std::string filePath, char** buffer)
{
	uint toReturn = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(filePath.c_str());

	if (fsFile == nullptr)
	{
		LOG(LOG_TYPE::ERRO, "File System error while opening file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
		return toReturn;
	}

	PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

	if (size > 0)
	{
		*buffer = new char[size + 1];
		toReturn = (uint) PHYSFS_readBytes(fsFile, *buffer, size);

		if (toReturn != size)
		{
			LOG(LOG_TYPE::ERRO, "File System error while reading from file '%s': %s", filePath.c_str(), PHYSFS_getLastError());
			RELEASE_ARRAY(*buffer);
			return toReturn;
		}
		else
		{
			//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
			(*buffer)[size] = '\0';
		}
	}

	if (PHYSFS_close(fsFile) == 0)
	{
		LOG(LOG_TYPE::ERRO, "File System error while closing from file '%s': %s", filePath.c_str(), PHYSFS_getLastError());
	}

	return toReturn;
}

uint LibraryManager::Save(std::string filePath, char* data, uint size, bool append)
{
	uint toReturn = 0;

	bool exists = Exists(filePath);

	PHYSFS_file* handler = nullptr;
	if (append) handler = PHYSFS_openAppend(filePath.c_str());
	else handler = PHYSFS_openWrite(filePath.c_str());

	if (handler == nullptr)
	{
		LOG(LOG_TYPE::ERRO, "FILE SYSTEM: Could not open file '%s' to write. ERROR: %s", filePath.data(), PHYSFS_getLastError());
		return toReturn;
	}

	if (size == 0) size = strlen(data);
	toReturn = PHYSFS_writeBytes(handler, data, size);

	//Logging
	if (toReturn == size)
	{
		if (exists)
		{
			if (append)
			{
				LOG(LOG_TYPE::SUCCESS, "FILE SYSTEM: Append %u bytes to file '%s'", toReturn, filePath.data());
			}
			else
			{
				LOG(LOG_TYPE::ATTENTION, "FILE SYSTEM: File '%s' overwritten with %u bytes", filePath.data(), toReturn);
			}
		}
		else
		{
			LOG(LOG_TYPE::SUCCESS, "FILE SYSTEM: New file '%s' created with %u bytes", filePath.data(), toReturn);
		}
	}
	else
	{
		LOG(LOG_TYPE::ERRO, "FILE SYSTEM: Could not write to file '%s'. ERROR: %s", filePath.data(), PHYSFS_getLastError());
	}

	//Close fileSystem
	if (PHYSFS_close(handler) == 0)
	{
		LOG(LOG_TYPE::ERRO, "File System error while closing from file '%s': %s", filePath.c_str(), PHYSFS_getLastError());
	}

	GetFilename(filePath);

	return toReturn;
}

uint LibraryManager::Copy(std::string filePath, std::string dir)
{
	uint size = 0;
	std::string saveDir = dir;
	saveDir += "/";

	//Copy name to new direction
	size_t pos = NormalizePath(filePath).find_last_of("/");
	if (pos != std::string::npos)
		saveDir += filePath.substr(pos + 1);

	char* buffer = nullptr;
	//Load(NormalizePath(filePath), &buffer);

	std::FILE* handler;
	fopen_s(&handler, filePath.c_str(), "rb");

	if (handler != nullptr)
	{
		fseek(handler, 0, SEEK_END);
		size = ftell(handler);
		rewind(handler);

		char* buffer = new char[size];
		size = fread(buffer, 1, size, handler);

		if (size > 0)
		{
			Save(saveDir, buffer, size);
		}


		RELEASE_ARRAY(buffer);
		fclose(handler);
	}

	return size;
}

void LibraryManager::SaveJSON(std::string filePath, std::string jsonDump)
{
	PHYSFS_file* handler = PHYSFS_openWrite(filePath.c_str());

	uint size = strlen(jsonDump.c_str());
	//PHYSFS_W
	PHYSFS_write(handler, jsonDump.c_str(), size, 1);

	if (PHYSFS_close(handler) == 0)
	{
		LOG(LOG_TYPE::ERRO, "File System error while closing from file '%s': %s", filePath.c_str(), PHYSFS_getLastError());
	}
}

std::string LibraryManager::LoadJSON(std::string filePath)
{
	std::string toReturn = "";
	char* buffer = nullptr;
	PHYSFS_file* fsFile = PHYSFS_openRead(filePath.c_str());

	if (fsFile == nullptr)
	{
		LOG(LOG_TYPE::ERRO, "File System error while opening file %s: %s\n", filePath.c_str(), PHYSFS_getLastError());
		return toReturn;
	}
	
	PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);
	buffer = new char[size + 1];

	uint what = PHYSFS_readBytes(fsFile, buffer, size);
	(buffer)[size] = '\0';
	toReturn = buffer;

	LOG(LOG_TYPE::ATTENTION, "Bytes-> %i this-> %s", what, toReturn.c_str());
	if (PHYSFS_close(fsFile) == 0)
	{
		LOG(LOG_TYPE::ERRO, "File System error while closing from file '%s': %s", filePath.c_str(), PHYSFS_getLastError());
	}

	return toReturn;
}