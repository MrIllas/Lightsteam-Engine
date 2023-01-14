#include "CompMaterial.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "LibraryFolder.h"
#include "Material.h"

#include "ShaderManager.h"
#include "Shader.h"
#include "ShaderUniform.h"

#include "ModuleResources.h"
#include "ResourceMaterial.h"

CompMaterial::CompMaterial(GameObject* owner, std::string uuid) : Component(owner, uuid)
{
	this->type = MATERIAL;

	resInstance = ResourceProperties::Instance();
}

CompMaterial::~CompMaterial()
{
 	if (this->material != nullptr) ResourceProperties::Instance()->resources.at(material->uuid)->DecreaseRC();
	else this->material = nullptr;
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
	UpdateDragDrop();
	ShaderSelectorCombo();
	ImGui::Separator();
	ShaderCustomGUI();
}

void CompMaterial::UpdateDragDrop()
{
	std::string btnTxt = "EMPTY";

	ImGui::Button(material == nullptr ? btnTxt.c_str() : material->name.c_str(), { 50, 50 });

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
		{
			IM_ASSERT(payload->DataSize == sizeof(LibraryItem));
			const LibraryItem item = *static_cast<const LibraryItem*>(payload->Data);

			if (item.extension == "material")
			{
				ResourceMaterial* res = (ResourceMaterial*)ResourceProperties::Instance()->resources.at(item.resUuid);

				//Clean previous (if there is one)
				if (this->material != nullptr)
				{
					ResourceMaterial* delRes = (ResourceMaterial*) ResourceProperties::Instance()->resources.at(material->uuid);
					delRes->RemoveMaterialToComp(this);

				}
				//Import
				if (res->GetMaterial() == nullptr)
				{
					res->ImportFromLibrary(this);
				}
				else
				{
					res->SetMaterialToComp(this);
					//res->IncreaseRC();
				}

			}
		}
	}
}

void CompMaterial::ShaderSelectorCombo()
{
	if (this->material == nullptr)return;

	std::string strSelected = "Select a shader";
	if (material->GetShader() != nullptr) strSelected = material->GetShader()->name;

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
				//Clean current Shader
				if (material->GetShader() != nullptr)
				{
					if (!material->GetShader()->uuid.empty())
					{
						ResourceShader* ress = (ResourceShader*)resInstance->resources.at(material->GetShader()->uuid);
						if (ress != nullptr) ress->DecreaseRC();
						if (ress->shader == nullptr) LOG(LOG_TYPE::ATTENTION, "RC 0: Unloading shader '%s' from memory!", ress->GetLibraryFile().c_str());
					}
				}

				//Set new shader
				material->SetShader(ShaderManager::ImportFromLibrary(shaderPool[i]));
				shaderPool[i]->IncreaseRC();
			}
		}

		ImGui::EndCombo();
	}
}

void CompMaterial::ShaderCustomGUI()
{
	if (this->material == nullptr || this->material->GetShader() == nullptr) return;
	
	//Save Material button
	if (ImGui::Button("Save"))
	{
		ResourceMaterial* res = (ResourceMaterial*) ResourceProperties::Instance()->resources.at(this->material->uuid);
		this->material->Save(res->GetLibraryFile());
	}

	//Custom Shader properties
	for (int i = 0; i < material->uniforms.size(); ++i)
	{
		if (material->uniforms[i] == nullptr) continue;
		if (material->uniforms[i]->name == "Projection" || material->uniforms[i]->name == "View" || material->uniforms[i]->name == "Model") continue;
		material->uniforms[i]->HandleShaderGUI();
	}
}
#pragma endregion


#pragma region Save/Load
nlohmann::ordered_json CompMaterial::SaveUnique(nlohmann::JsonData data)
{
	if (material != nullptr)
		data.SetString("Material Uuid", material->uuid);

	return data.data;
}

void CompMaterial::LoadUnique(nlohmann::JsonData data)
{
	std::string materialUuid = data.GetString("Material Uuid");
	ResourceMaterial* res = nullptr;

	ResourceProperties* resP = ResourceProperties::Instance();
	if (ResourceProperties::Instance()->resources.count(materialUuid) == 1)
		res = (ResourceMaterial*)ResourceProperties::Instance()->resources.at(materialUuid);
	
	if (res != nullptr)
	{
		if (res->GetMaterial() == nullptr)
			res->ImportFromLibrary(this);
		else
		{
			res->SetMaterialToComp(this);
			/*this->material = res->material;
			res->IncreaseRC();*/
		}
			
	}
	
}
#pragma endregion