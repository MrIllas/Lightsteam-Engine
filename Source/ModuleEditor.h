#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "SegmentAbout.h"

class Application;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);

	virtual ~ModuleEditor();

	bool Init();
	bool Start();
	bool CleanUp();

	UpdateStatus PostUpdate();

	void DrawEditorGui();

private:
	void BeginRender();
	void EndRender();

	void UpdateSegments();

	void MainMenuBar();



private:
	bool showAppConsole = false;
	bool showAppConfiguration = false;
	bool showAppAbout = false;

	SegmentAbout* segAbout = nullptr;
};

#endif // __ModuleEditor_H__