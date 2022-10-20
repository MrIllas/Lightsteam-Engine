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
		ClearGUI();
		ImGui::SameLine();
		LogGUI();
		ImGui::SameLine();
		OptionsGUI();

		if (ImGui::BeginChild("TextArea"))
		{
			for (int i = 0; i < loggs->Size(collapse); ++i)
			{
				//Filter
				LOG_TYPE aux = loggs->GetType(i, collapse);
				if (aux == LOG_TYPE::SUCCESS && !logSuccess) continue;
				if (aux == LOG_TYPE::ERRO && !logError) continue;
				if (aux == LOG_TYPE::ATTENTION && !logAttention) continue;
				if (aux == LOG_TYPE::ENGINE && !logEngine) continue;

				//Time
				if (showTime)
				{
					ImGui::Text(loggs->GetTime(i, collapse));
					ImGui::SameLine();
				}
				
				

				//LOG COLOUR
				ImVec4 colour;
				switch (aux)
				{
				case LOG_TYPE::NONE:
					colour = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
					break;
				case LOG_TYPE::SUCCESS:
					colour = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
					break;
				case LOG_TYPE::ATTENTION:
					colour = ImVec4(1.0f, 0.75f, 0.0f, 1.0f);
					break;
				case LOG_TYPE::ERRO:
					colour = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
					break;
				}

				//Collapse NUMBER
				if (collapse)
				{
					std::string str = "[" + std::to_string(loggs->GetCollapseCount(i)) + "]";

					if (aux == LOG_TYPE::ENGINE) ImGui::Text(str.c_str());
					else ImGui::TextColored(colour, str.c_str());
					ImGui::SameLine();
				}

				//LOG Message
				if (aux == LOG_TYPE::ENGINE) ImGui::Text(loggs->GetString(i, collapse));
				else ImGui::TextColored(colour, loggs->GetString(i, collapse));
				
			}
			//Scroll
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

void SegmentConsole::ClearGUI()
{
	if (ImGui::Button("Clear"))
	{
		loggs->Delete();
		loggs = loggs->Instance();
	}
}

void SegmentConsole::LogGUI()
{
	if (ImGui::Button("Log Types")) ImGui::OpenPopup("Log Types");

	if (ImGui::BeginPopup("Log Types"))
	{
		ImGui::Checkbox("SUCCESS", &logSuccess);
		ImGui::Checkbox("ERROR", &logError);
		ImGui::Checkbox("ATTENTION", &logAttention);
		ImGui::Checkbox("ENGINE", &logEngine);

		ImGui::EndPopup();
	}
}

void SegmentConsole::OptionsGUI()
{
	if (ImGui::Button("Options")) ImGui::OpenPopup("Options");

	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Collapse", &collapse);
		ImGui::Checkbox("Show time", &showTime);

		ImGui::EndPopup();
	}
}