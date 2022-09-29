#include "SegmentConsole.h"

#include "Loggs.h"

SegmentConsole::SegmentConsole(bool enabled) : Segment(enabled)
{
	name = "Console";

	loggs = Loggs::Instance();
}

SegmentConsole::~SegmentConsole()
{
	loggs->Delete();
}

void SegmentConsole::Update()
{
	if (loggs->Size() > lastSize)
	{
		lastSize = loggs->Size();
		scrollDown = true;
	}

	if (ImGui::Begin(name.c_str()))
	{
		
		if (ImGui::BeginChild("TextArea"))
		{
			
			for (int i = 0; i < loggs->Size(); ++i)
			{
				ImGui::Text(loggs->Get(i));
			}
			if(scrollDown)
			{
				ImGui::SetScrollHereY(1.0f);
				scrollDown = false;
			}
				
		}
		ImGui::EndChild();

	}
	ImGui::End();
}