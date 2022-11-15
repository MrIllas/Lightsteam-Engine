#include "CompTexture.h"

#include "TextureImporter.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

CompTexture::CompTexture(GameObject* owner, std::string uuid) : Component(owner, uuid)
{
	this->type = MATERIAL;

	isCheckers = false;
}

CompTexture::~CompTexture()
{

}

void CompTexture::Init()
{
	checkersTexture = TextureImporter::checkers;
	//checkersTexture.path = "DEBUG TEXTURE(CHECKERS)";
}

void CompTexture::Update()
{
	if (!active) return;
}

void CompTexture::UpdateGUI()
{
	ImGui::NewLine();
	ImGui::Checkbox("Checkers Texture", &isCheckers);

	//Texture Path
	ImGui::NewLine();
	std::string txt = "Texture Path: ";
	if (this->texture.w == 0 && this->texture.h == 0 && !isCheckers) txt += "No texture loaded, using debug texture";
	else if (!isCheckers) txt += texture.path;
	else txt += checkersTexture.path;
	ImGui::Text(txt.c_str());

	//Texture Width and Height
	txt = "Size: ";
	if (isCheckers || (this->texture.w == 0 && this->texture.h))
	{
		txt += std::to_string(checkersTexture.w);
		txt += " x ";
		txt += std::to_string(checkersTexture.h);
	}
	else
	{
		txt += std::to_string(texture.w);
		txt += " x ";
		txt += std::to_string(texture.h);
	}
	ImGui::Text(txt.c_str());

	//Texture ID
	txt = "Texture ID: ";
	if (isCheckers || (this->texture.w == 0 && this->texture.h)) txt += std::to_string(checkersTexture.id);
	else txt += std::to_string(texture.id);
	ImGui::Text(txt.c_str());
}

void CompTexture::SetTexture(Texture texture)
{
	this->texture = texture;
}

void CompTexture::SetTexture(unsigned int id, std::string path)
{
	this->texture.id = id;
	this->texture.path = path;
}

Texture CompTexture::GetTexture()
{
	if (isCheckers) return this->checkersTexture;
	else if (this->texture.w == 0 && this->texture.h == 0) return this->checkersTexture;
	else return this->texture;
}