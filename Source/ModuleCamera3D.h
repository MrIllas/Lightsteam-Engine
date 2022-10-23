#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"

#include "Camera.h"

struct CameraProperties
{
public:
	Camera editorCamera;

	CameraProperties();

	static CameraProperties* Instance();

	static void Delete();

private:
	static CameraProperties* cProps;

};

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init() override;
	bool Start();
	UpdateStatus Update();
	bool CleanUp();

	/*void Look(const float3 &Position, const float3&Reference, bool RotateAroundReference = false);
	void LookAt(const float3&Spot);
	void Move(const float3&Movement);*/
	//float* GetViewMatrix();

	void SaveSettingsData(pugi::xml_node& save) override;

	void LoadSettingsData(pugi::xml_node& load) override;


private:

	void SceneCameraInput();
	//void CalculateViewMatrix();

public:
	
	//float3 X, Y, Z, Position, Reference;

private:

	

	CameraProperties* cProps = nullptr;

	//mat4x4 ViewMatrix;//View camera
	//float4x4 ViewMatrixInverse;

	//Vehicle* target = nullptr;
	//float3 offset;
};

#endif // !__MODULECAMERA3D_H__