#include "SegmentLibrary.h"

#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "ModuleResources.h"
#include "LibraryFolder.h"
#include "LibraryManager.h"

#include "ImGui/imgui_internal.h"
#include "ImGuiUtils.h"

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
	sInstance = SceneProperties::Instance();
	resInstance = ResourceProperties::Instance();

	currentFolder = fsIntance->rootFolder;
	fsIntance->currentFolder = currentFolder;
	//Checks firsts when the app loads 
	resInstance->requestFolderFileCheck = true;

	leftWin = 0;
	rightWin = 0;
}

void SegmentLibrary::Update()
{
	if (ImGui::Begin(name.c_str()))
	{
		
		ImVec2 winSize = ImGui::GetContentRegionAvail();
		leftWin = 150;
		rightWin = winSize.x - 150;
		
		

		ImGui::DrawSplitter(0, 10, &leftWin, &rightWin, 150, 200);
		
		if (leftWin > 0.0f)
		{
			Hierarchy();
		}

		ImGui::SameLine();

		if (rightWin > 0.0f)
		{
			BoxView();
		}

		
	}
	ImGui::End();

	fsIntance->currentFolder = currentFolder;
}

void SegmentLibrary::Hierarchy()
{
	if (ImGui::BeginChild("Library##Hierarchy", ImVec2(leftWin, 0), true))
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
	static float padding = 8.0f;
	static float itemSize = 64;
	float cellSize = itemSize + padding;

	if (ImGui::BeginChild("Library##BoxView", ImVec2(rightWin, 0), true))
	{
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / (cellSize + padding*2));
		ImGui::Columns(columnCount, 0, false);

		//Go to previous folder topping the root Library folder
		if (currentFolder != fsIntance->rootFolder)
		{
			if (ImGui::Button("<--", { cellSize, cellSize})) currentFolder = currentFolder->parent;
			ImGui::NextColumn();
		}

		//Iterate folders
		for (int i = 0; i < currentFolder->children.size(); ++i)
		{
			if (ImGui::Button(currentFolder->children[i]->name.c_str(), { cellSize, cellSize }))
			{
				LibraryManager::FolderSystemUpdate(currentFolder->children[i]);
				resInstance->requestFolderFileCheck = true;
				currentFolder = currentFolder->children[i];
			}
			else ImGui::Text(currentFolder->children[i]->name.c_str());
			ImGui::NextColumn();
		}

		//Iterate files
		for (int k = 0; k < currentFolder->libItem.size(); ++k)
		{
			ImGui::PushID(k);
			if (ImGui::ImageButton(0, {cellSize, cellSize}))
			{
				switch (str2int(currentFolder->libItem[k]->extension.c_str()))
				{
					case str2int("dds"):
					case str2int("png"):

						break;
					default:
						
						break;
				}
			}
			
			//Drag
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ContentBrowserItem", currentFolder->libItem[k], sizeof(LibraryItem));
				ImGui::Text(currentFolder->libItem[k]->name.c_str());

				ImGui::EndDragDropSource();
			}


			ImGui::Text(currentFolder->libItem[k]->name.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);
		//Options

		ImGui::SliderFloat("Zoom", &itemSize, 16, 128, "%.0f");
		ImGui::SliderFloat("Pad", &padding, 0, 32);

	}
	ImGui::EndChild();
}
