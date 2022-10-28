#include "CompTexture.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

CompTexture::CompTexture(GameObject* owner) : Component(owner)
{
	this->type = MATERIAL;
}

CompTexture::~CompTexture()
{

}

void CompTexture::Update()
{

}

void CompTexture::UpdateGUI()
{
	if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_Leaf))
	{
		ImGui::Checkbox("Active##Material", &this->active);
	}
}