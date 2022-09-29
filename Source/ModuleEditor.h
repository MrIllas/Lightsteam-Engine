#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"

#include <iostream>
#include <vector>

class Application;

class Segment;
class SegmentAbout;
class SegmentConfiguration;

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

	void SaveSettingsData(pugi::xml_node& save) override;

	void LoadSettingsData(pugi::xml_node& load) override;

private:
	bool showAppConsole = false;
	bool showAppAbout = false;

	std::vector<Segment*> segments;
};

#endif // __ModuleEditor_H__