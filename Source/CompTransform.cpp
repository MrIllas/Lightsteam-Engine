#include "CompTransform.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

CompTransform::CompTransform(GameObject* owner) : Component (owner)
{
	this->type = CO_TYPE::TRANSFORM;
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
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_Leaf))
	{
		ImGui::Checkbox("Active##Transform", &active);
	}
}