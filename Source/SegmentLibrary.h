#pragma once
#include "Segment.h"

//#include "LibraryManager.h"

struct FileSystemProperties;
struct SceneProperties;
struct ResourceProperties;
class LibraryFolder;

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

private:
	float leftWin;
	float rightWin;

	FileSystemProperties* fsIntance = nullptr;
	ResourceProperties* resInstance = nullptr;
	SceneProperties* sInstance = nullptr;
	LibraryFolder* currentFolder = nullptr;
};

