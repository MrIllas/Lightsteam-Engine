#pragma once
#include "Segment.h"

//#include "LibraryManager.h"

struct FileSystemProperties;
struct SceneProperties;
struct ResourceProperties;
class LibraryFolder;
struct LibraryItem;

class SegmentLibrary : public Segment
{
public:
	SegmentLibrary(bool enabled = false);
	~SegmentLibrary();

	void Start();
	
	void Update();
	
private:
	void Hierarchy();
	void BoxView();

	void ExecuteItemActive(LibraryItem* item, float cellSize);

private:
	float leftWin;
	float rightWin;

	FileSystemProperties* fsIntance = nullptr;
	ResourceProperties* resInstance = nullptr;
	SceneProperties* sInstance = nullptr;
	LibraryFolder* currentFolder = nullptr;

	std::string* package = nullptr;
};

