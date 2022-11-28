#pragma once

#include <string>
#include <vector>

struct LibraryItem
{
	LibraryItem(std::string path, std::string name)
	{
		this->path = path;
		this->name = name;
		//extension = GetExten
	}
	bool hasMeta = false;
	std::string path;
	std::string name;
	std::string extension;
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
		for (int i = 0; i < children.size(); ++i)
		{
			RELEASE(children[i]);
		}

		for (int i = 0; i < libItem.size(); ++i)
		{
			RELEASE(libItem[i]);
		}
	}

	std::string path;
	std::string name;
	LibraryFolder* parent; //If empty it means its root

	std::vector<LibraryFolder*> children;

	std::vector<LibraryItem*> libItem; //Contains all files
};