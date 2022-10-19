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
		

		if (ImGui::Button("Clear"))
		{
			loggs->Delete();
			loggs = loggs->Instance();
		}
		ImGui::SameLine();
		if (ImGui::Button("Log Types")) ImGui::OpenPopup("Options");
		
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("SUCCESS", &logSuccess);
			ImGui::Checkbox("ERROR", &logError);
			ImGui::Checkbox("ATTENTION", &logAttention);
			ImGui::Checkbox("ENGINE", &logEngine);

			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Checkbox("Collapse", &collapse);
		ImGui::SameLine();
		ImGui::Checkbox("Show time", &showTime);

		if (ImGui::BeginChild("TextArea"))
		{
			for (int i = 0; i < loggs->Size(); ++i)
			{
				LOG_TYPE aux = loggs->GetType(i);
				if (aux == LOG_TYPE::SUCCESS && !logSuccess) continue;
				if (aux == LOG_TYPE::ERRO && !logError) continue;
				if (aux == LOG_TYPE::ATTENTION && !logAttention) continue;
				if (aux == LOG_TYPE::ENGINE && !logEngine) continue;

				if (showTime)
				{
					ImGui::Text(loggs->GetTime(i));
					ImGui::SameLine();
				}
				

				switch (aux)
				{
				case LOG_TYPE::NONE:
					ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), loggs->GetString(i));
					break;
				case LOG_TYPE::SUCCESS:
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), loggs->GetString(i));
					break;
				case LOG_TYPE::ATTENTION:
					ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.0f, 1.0f), loggs->GetString(i));
					break;
				case LOG_TYPE::ERRO:
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), loggs->GetString(i));
					break;
				case LOG_TYPE::ENGINE:
					ImGui::Text(loggs->GetString(i));
					break;
				}			
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