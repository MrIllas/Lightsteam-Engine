#pragma once
#include "Segment.h"

//#include "LibraryManager.h"

struct FileSystemProperties;
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
	FileSystemProperties* fsIntance = nullptr;
	LibraryFolder* currentFolder = nullptr;
};

