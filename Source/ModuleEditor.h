#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"

#include <iostream>
#include <vector>

class Application;

class Segment;
class SegmentShaderText;


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

	void RequestShaderTextSwitch(std::string shaderResourceUuid);
	void ForceClose();

	bool GetShaderTextRequest() 
	{ 
		if (requestShaderTextSwitch)
		{
			requestShaderTextSwitch = false;
			return true;
		}
		return false; 
	}

	std::string GetShaderTextRequestUuid()
	{
		std::string aux = requestShaderTextUuid;
		requestShaderTextUuid = "NULL";
		return aux;
	}

private:
	static EditorProperties* instance;

	bool requestShaderTextSwitch = false;
	std::string requestShaderTextUuid;

};

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);

	virtual ~ModuleEditor();

	bool Init();
	bool Start();
	bool CleanUp();

	UpdateStatus Update();
	UpdateStatus PostUpdate();

	void DrawEditorGui();

private:
	void BeginRender();
	void EndRender();

	void RequestSwitchHandler();
	void UpdateSegments();

	void MainMenuBar();
	void SubMenuBar();
	void FileDialogMenu();

	void SaveSettingsData(pugi::xml_node& save) override;

	void LoadSettingsData(pugi::xml_node& load) override;

private:
	std::vector<Segment*> segments;
	SegmentShaderText* segmentShaderText = nullptr;
	int segmentViewPoolOff;
	int segmentGameCount;

	EditorProperties* eProps = nullptr;
};

#endif // __ModuleEditor_H__