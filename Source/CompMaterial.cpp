#include "CompMaterial.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "ShaderManager.h"

#include "ModuleResources.h"

CompMaterial::CompMaterial(GameObject* owner, std::string uuid) : Component(owner, uuid)
{
	this->type = TESTING;

	resInstance = ResourceProperties::Instance();
}

CompMaterial::~CompMaterial()
{
	
}

void CompMaterial::Init()
{
	//resInstance = ResourceProperties::Instance();
}

void CompMaterial::Update()
{

}

#pragma region GUI Related
void CompMaterial::UpdateGUI()
{
	ShaderGUICombo();
	ImGui::Separator();
	ImGui::Text("Test text");
}

void CompMaterial::ShaderGUICombo()
{
	std::string strSelected = "NONE";
	if(shader != nullptr) strSelected = shader->name;

	std::vector<ResourceShader*> shaderPool;


	if (ImGui::BeginCombo("##ShaderCombo", strSelected.c_str()))
	{
		shaderPool = resInstance->GetResourcePool<ResourceShader>(RESOURCE_TYPE::SHADER);

		std::string aux;

		for (int i = 0; i < shaderPool.size(); ++i)
		{
			aux = shaderPool[i]->GetName();
			aux += "##";
			aux += shaderPool[i]->GetUUID();
			if (ImGui::Selectable(aux.c_str()))
			{
				//Do the thing
				this->shader = ShaderManager::ImportFromLibrary(shaderPool[i]);
				shaderPool[i]->IncreaseRC();
			}
		}

		ImGui::EndCombo();
	}
}

#pragma endregion


#pragma region Save/Load
nlohmann::ordered_json CompMaterial::SaveUnique(nlohmann::JsonData data)
{
	data.SetString("Shader Uuid", shader->uuid);

	return data.data;
}

void CompMaterial::LoadUnique(nlohmann::JsonData data)
{

}
#pragma endregion