#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"

#include <iostream>
#include <vector>

class Application;

class Segment;

enum COLORMODE
{
	LightMode = 0,
	DarkMode,
	ClassicMode
};

struct EditorProperties
{
public:
	COLORMODE colorMode = COLORMODE::DarkMode;

	EditorProperties();

	static EditorProperties* Instance();

	static void Delete();

	void SwitchColorMode();

private:
	static EditorProperties* instance;
};

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
	std::vector<Segment*> segments;

	EditorProperties* eProps = nullptr;
};

#endif // __ModuleEditor_H__