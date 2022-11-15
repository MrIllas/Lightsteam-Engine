#include "CompTransform.h"

#include "MathGeoLib/include/MathGeoLib.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

CompTransform::CompTransform(GameObject* owner, std::string uuid) : Component (owner, uuid)
{
	this->type = CO_TYPE::TRANSFORM;

	position = float3(.0f, .0f, .0f);
	//rotation = Quat::identity;
	rotation = float3(.0f, .0f, .0f);
	localScale = float3(1.0f, 1.0f, 1.0f);
}

CompTransform::~CompTransform()
{

}

void CompTransform::Init()
{
	
}

void CompTransform::Update()
{

}

void CompTransform::UpdateGUI()
{
	ImGui::NewLine();

	ImGui::DragFloat3("Position", &position[0], 0.25f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoInput);
	ImGui::DragFloat3("Rotation", &rotation[0]);
	ImGui::DragFloat3("Scale", &localScale[0], 0.25f, 0.0f, 0.0f, "%.2f");
}

float4x4 CompTransform::GetWorldMatrix()
{
	math::Quat q = Quat::FromEulerXYZ(math::DegToRad(rotation.x), math::DegToRad(rotation.y), math::DegToRad(rotation.z));
	float4x4 toReturn = float4x4::FromTRS(position, q.ToFloat4x4(), localScale);
	toReturn.Transpose();
	return toReturn;
}

Quat CompTransform::GetRotationAsQuat()
{
	return Quat::FromEulerXYZ(math::DegToRad(rotation.x), math::DegToRad(rotation.y), math::DegToRad(rotation.z));
}