#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"

class Application;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);

	virtual ~ModuleEditor();

	bool Init();
	bool CleanUp();

	UpdateStatus PostUpdate();

	void DrawEditorGui();

private:
	void BeginRender();
	void EndRender();

	UpdateStatus MainMenuBar();
};

#endif // __ModuleEditor_H__