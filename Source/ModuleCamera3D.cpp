#include <iostream>
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "CompTransform.h"

#include "MathGeoLib/include/Math/Quat.h"

#include "ImGui/imgui_internal.h"
#include "ImGuizmo/ImGuizmo.h"

#pragma region CameraProperties
CameraProperties::CameraProperties()
{
}

CameraProperties* CameraProperties::Instance()
{
	if (cProps == nullptr) cProps = new CameraProperties();

	return cProps;
}

void CameraProperties::Delete()
{
	if (cProps != nullptr)
	{
		RELEASE(cProps);
	}
}

CameraProperties* CameraProperties::cProps = nullptr;
#pragma endregion Camera Properties Singleton Struct

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Camera3D";
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init()
{
	cProps = CameraProperties::Instance();

	return true;
}


// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	bool ret = true;

	sProps = SceneProperties::Instance();

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG(LOG_TYPE::ENGINE, "Cleaning camera");

	CameraProperties::Delete();

	return true;
}

// -----------------------------------------------------------------
UpdateStatus ModuleCamera3D::Update()
{
	SceneCameraInput();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::SceneCameraInput()
{
	if (!cProps->isMouseOnScene) return;
	GameObject* selectedGO = sProps->GetSelectedGO();
	CompTransform* selected = nullptr;
	if (selectedGO != nullptr) selected = selectedGO->GetComponent<CompTransform>(TRANSFORM);

	float3 empty = { 0,0,0 };
	float3 newPos(.0f, .0f, .0f);
	Quat lookingDir = Quat::identity;
	cProps->editorCamera.frustum.WorldMatrix().Decompose(empty, lookingDir, empty);

	// Mouse motion ----------------
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float DeltaX = 0;
	float DeltaY = 0;

	float Sensitivity = 0.25f;

	float speed = 6.0f, zoomSpeed = 8.0f;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2.0f, zoomSpeed *= 2.0f;
	speed *= App->fps;
	zoomSpeed *= App->fps;

	//ZOOM-IN/ZOOM-OUT
	int mouseZ = App->input->GetMouseZ();
	if (mouseZ != 0)
	{
		newPos += cProps->editorCamera.frustum.front * zoomSpeed * mouseZ;
	}

	//WASD movement
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{	
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += cProps->editorCamera.frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= cProps->editorCamera.frustum.front * speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= cProps->editorCamera.frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += cProps->editorCamera.frustum.WorldRight() * speed;

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y -= speed;

		cProps->editorCamera.Position -= cProps->editorCamera.Reference;

		if (dx != 0)
		{
			DeltaX = math::DegToRad((float)dx * Sensitivity * 0.75f);

			Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle(float3(.0f, 1.0f, .0f), DeltaX);

			lookingDir = rotation * lookingDir;
		}

		if (dy != 0)
		{
			DeltaY = math::DegToRad((float)dy * Sensitivity * 0.75f);

			Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle(float3(1.0f, .0f, .0f), DeltaY);

			lookingDir = lookingDir * rotation;
		}

		float4x4 aux = cProps->editorCamera.frustum.WorldMatrix();
		aux.SetRotatePart(lookingDir.Normalized());
		cProps->editorCamera.frustum.SetWorldMatrix(aux.Float3x4Part());
	}

	//Orbit
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && (App->input->GetKey(SDL_SCANCODE_LALT) || App->input->GetKey(SDL_SCANCODE_RALT)) && selected != nullptr)
	{
		float3 center = selected->position;

		float distance = cProps->editorCamera.frustum.pos.Distance(center);

		if(dx != 0)
		{
			DeltaX = (float)dx * Sensitivity;

			Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle(float3(.0f, 1.0f, .0f), DeltaX * DEGTORAD);

			lookingDir = rotation * lookingDir;
		}

		if (dy != 0)
		{
			DeltaY = (float)dy * Sensitivity;

			Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle(float3(1.0f, .0f, .0f), DeltaY * DEGTORAD);

			lookingDir = lookingDir * rotation;
		}

		float4x4 aux = cProps->editorCamera.frustum.WorldMatrix();
		aux.SetRotatePart(lookingDir.Normalized());
		cProps->editorCamera.frustum.SetWorldMatrix(aux.Float3x4Part());

		cProps->editorCamera.frustum.pos = center + (cProps->editorCamera.frustum.front * -distance);
	}

	//Focus
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && selected != nullptr)
	{
		float3 pos = selected->position;
		float3 dir(.0f, 0.f, 0.f);
		pos += float3(5.0f, .0f, .0f);
		cProps->editorCamera.Move(pos);
		cProps->editorCamera.LookAt(dir);
	}

	cProps->editorCamera.Reference += newPos;
	cProps->editorCamera.frustum.pos += newPos;

	
	//Set GO float4x4 as scene camera
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && selected != nullptr)
	{
		selected->position = cProps->editorCamera.frustum.pos;
		float3 rot = cProps->editorCamera.frustum.WorldMatrix().ToEulerXYZ();
		selected->rotation = float3(math::RadToDeg(rot.x), math::RadToDeg(rot.y), math::RadToDeg(rot.z));
	}

	//Go selector
	if (App->input->App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGuizmo::IsOver())
	{
		LineSegment ray = cProps->editorCamera.ScreenPointToRay(cProps->mouseScreenPos);

		//if (!sProps->Intersect(sProps->root, ray)) sProps->UnselectGO();
		sProps->Intersect(sProps->root, ray);
	}
}


#pragma region Save/Load Settings

void ModuleCamera3D::LoadSettingsData(pugi::xml_node& load)
{

}

void ModuleCamera3D::SaveSettingsData(pugi::xml_node& save)
{

}

#pragma endregion Save & Load of Settings