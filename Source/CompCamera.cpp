#include "CompCamera.h"

#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "CompTransform.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

CompCamera::CompCamera(GameObject* owner, std::string uuid) : Component(owner, uuid)
{
	this->type = CAMERA;

	camInstance = CameraProperties::Instance();

	if (camInstance->gameCameras.size() == 0)
	{
		isMainCamera = true;
		camInstance->mainCameraId = 0;
	}
	else isMainCamera = false;

	//Includes camera to the list of game cameras.
	camInstance->gameCameras.emplace_back(this);
}

CompCamera::~CompCamera()
{

}

void CompCamera::Init()
{

}

void CompCamera::Update()
{
	CompTransform* transform = owner->GetComponent<CompTransform>(TRANSFORM);

	if (transform != nullptr)
	{

		float4x4 aux = camera.frustum.WorldMatrix();
		aux.SetRotatePart(transform->GetRotationAsQuat());
		camera.frustum.SetWorldMatrix(aux.Float3x4Part());

		camera.Position = transform->position;
		camera.Reference = transform->position;
		camera.frustum.pos = transform->position;
	}
	
}

void CompCamera::UpdateGUI()
{
	if (isMainCamera) ImGui::Text("Im the main camera!");
}