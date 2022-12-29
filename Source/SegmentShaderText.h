#pragma once
#include "Segment.h"

#include "ImGuiColorTextEdit/TextEditor.h"

//#include <string>

class ResourceShader;
struct ResourceProperties;

struct EditorData
{
	std::string lastSave;
	bool edited = false;

	bool compilationError = false;
};

class SegmentShaderText : public Segment
{
public:
	SegmentShaderText(bool enabled = false);
	~SegmentShaderText() override;

	void Start() override;
	void Update() override;

	void SetResource(std::string resourceUuid);

private:
	void MenuBar();

	void SaveAction();

private:
	TextEditor editor;
	TextEditor::LanguageDefinition lang;
	EditorData editorData;

	ResourceShader* currentResource = nullptr;

	ResourceProperties* resInstance = nullptr;
};

