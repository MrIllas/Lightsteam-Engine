#include "SegmentShaderText.h"

#include "ModuleResources.h"
#include "ShaderManager.h"


SegmentShaderText::SegmentShaderText(bool enabled) : Segment(enabled)
{
	name = "GLSL Editor";
}

SegmentShaderText::~SegmentShaderText()
{

}

void SegmentShaderText::Start()
{
	resInstance = ResourceProperties::Instance();

	lang = TextEditor::LanguageDefinition::GLSL();

	editor.SetLanguageDefinition(lang);
}

void SegmentShaderText::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

	//Key handling
	if (editor.GetSaveRequest()) SaveAction();

	if (editor.IsTextChanged())
	{
		editorData.edited = true;
	}
	if (editorData.edited)
	{
		flags |= ImGuiWindowFlags_UnsavedDocument;
	}

	if (ImGui::Begin(name.c_str(), &enabled, flags))
	{
		MenuBar();

		//Editor Render
		auto cpos = editor.GetCursorPosition();
		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "Und" : " ",
			editor.GetLanguageDefinition().mName.c_str(), currentResource->GetAssetsFile().c_str());

		//Compilation error feedback
		if (editorData.compilationError)
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Compilation error: Check console!");
		}

		editor.Render("TextEditor");
	}
	ImGui::End();
}

void SegmentShaderText::MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "Ctrl-S"))
			{
				SaveAction();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor.IsReadOnly();

			if (ImGui::MenuItem("Undo", "Ctrl-Z", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				editor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				editor.SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				editor.SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void SegmentShaderText::SaveAction()
{
	editorData.edited = false;

	//Delete previous binary
	if (LibraryManager::Exists(currentResource->GetLibraryFile()))
		LibraryManager::RemoveFile(currentResource->GetLibraryFile());

	LibraryManager::SaveJSON(currentResource->GetAssetsFile(), editor.GetText());//Save .shader
	editorData.compilationError = ShaderManager::ImportToLibrary(currentResource);//Compile
}

void SegmentShaderText::SetResource(std::string resourceUuid)
{
	//Clean previous data
	if (this->currentResource != nullptr)
	{
		this->currentResource->isOpenOnEditor = false;
	}

	this->currentResource = (ResourceShader*) ResourceProperties::Instance()->resources.at(resourceUuid);
	this->currentResource->isOpenOnEditor = true;

	//Sets text on editor
	char* text = nullptr;
	LibraryManager::Load(currentResource->GetAssetsFile(), &text);

	editorData.lastSave = text;
	editor.SetText(text);

	RELEASE(text);
}