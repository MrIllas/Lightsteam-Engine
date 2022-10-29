#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"

#include "Camera.h"

struct CameraProperties
{
public:
	Camera editorCamera;
	bool isMouseOnScene = false;

	CameraProperties();

	static CameraProperties* Instance();

	static void Delete();

private:
	static CameraProperties* cProps;

};

struct SceneProperties;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init() override;
	bool Start();
	UpdateStatus Update();
	bool CleanUp();

	void SaveSettingsData(pugi::xml_node& save) override;

	void LoadSettingsData(pugi::xml_node& load) override;


private:

	void SceneCameraInput();

private:
	CameraProperties* cProps = nullptr;
	SceneProperties* sProps = nullptr;
};

#endif // !__MODULECAMERA3D_H__