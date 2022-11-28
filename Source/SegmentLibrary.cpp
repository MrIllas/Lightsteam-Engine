#include "SegmentLibrary.h"

#include "ModuleFileSystem.h"
#include "LibraryFolder.h"

SegmentLibrary::SegmentLibrary(bool enabled) : Segment(enabled)
{
	name = "Library";
}

SegmentLibrary::~SegmentLibrary()
{

}

void SegmentLibrary::Start()
{
	fsIntance = FileSystemProperties::Instance();

	currentFolder = fsIntance->rootFolder;
}

void SegmentLibrary::Update()
{
	if (ImGui::Begin(name.c_str()))
	{
		Hierarchy();
		BoxView();
	}
	ImGui::End();
}

void SegmentLibrary::Hierarchy()
{
	if (ImGui::BeginChild("Library##Hierarchy", ImVec2(0, 0), true))
	{
		for (int i = 0; i < currentFolder->children.size(); ++i)
		{
			ImGui::Text(currentFolder->children[i]->name.c_str());
		}
	}
	ImGui::EndChild();
}

void SegmentLibrary::BoxView()
{
	if (ImGui::BeginChild("Library##BoxView"))
	{
		for (int i = 0; i < currentFolder->children.size(); ++i)
		{
			ImGui::Text(currentFolder->children[i]->name.c_str());
		}
	}
	ImGui::EndChild();
}
