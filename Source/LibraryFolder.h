#pragma once

#include "Globals.h"

#include <string>
#include <vector>

struct LibraryItem
{
public:
	LibraryItem(std::string path, std::string name, std::string extension, bool hasMeta)
	{
		this->path = path;
		this->name = name;
		this->extension = extension;
		this->hasMeta = hasMeta;
		
		//Get crude path (Path without name)
		size_t pos = path.find_last_of("/");
		if (pos != std::string::npos)
			crudePath = path.erase(pos + 1);
	}
	bool hasMeta = false;
	std::string resUuid = "";
	std::string path;
	std::string crudePath;
	std::string name;
	std::string extension;
	bool active = false;


	std::string GetMeta() const 
	{
		std::string toReturn = path;
		return toReturn += ".meta";
	}
};

class LibraryFolder
{
public:
	LibraryFolder(std::string path, std::string name, LibraryFolder* parent)
	{
		this->path = path;
		this->name = name;
		this->parent = parent;
	}
	~LibraryFolder()
	{
		CleanUp();
	}

	std::string path;
	std::string name;
	LibraryFolder* parent; //If empty it means its root

	std::vector<LibraryFolder*> children;

	std::vector<LibraryItem*> libItem; //Contains all files


public:
	void CleanUp()
	{
		for (int i = 0; i < children.size(); ++i)
		{
			RELEASE(children[i]);
		}
		children.clear();

		for (int i = 0; i < libItem.size(); ++i)
		{
			RELEASE(libItem[i]);
		}
		libItem.clear();
	}

	bool ContainsPath(std::string path)
	{
		for (int i = 0; i < libItem.size(); ++i)
		{
			if (libItem[i]->path == path) return true;
		}

		for (int i = 0; i < children.size(); ++i)
		{
			if (children[i]->path == path) return true;
		}

		return false;
	}

	LibraryItem* GetFile(std::string path)
	{
		for (int i = 0; i < libItem.size(); ++i)
		{
			if (libItem[i]->path == path) return libItem[i];
		}

		return nullptr;
	}
};