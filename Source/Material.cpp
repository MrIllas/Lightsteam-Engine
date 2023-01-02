#include "Material.h"

#include "ModuleResources.h"
#include "ResourceMaterial.h"
#include "ResourceShader.h"
#include "ShaderManager.h"

#include "LibraryManager.h"

#include "Shader.h"
Material::Material(std::string name)
{
	this->name = name;
}

Material::~Material()
{

}

#pragma region Save&Load

void Material::Save(std::string filepath)
{
	nlohmann::JsonData data;
	
	data.SetString("Name", this->name);
	data.SetString("UUID", this->uuid);

	if (shader != nullptr)
		data.SetString("Shader Uuid", shader->uuid);

	LibraryManager::SaveJSON(filepath, data.data.dump(4));
}

void Material::Load(nlohmann::JsonData data)
{
	this->name = data.GetString("Name");
	this->uuid = data.GetString("UUID");

	//Load Shader
	std::string shaderUuid = data.GetString("Shader Uuid");

	ResourceShader* res = nullptr;
	if (ResourceProperties::Instance()->resources.count(shaderUuid) == 1)
		res = (ResourceShader*)ResourceProperties::Instance()->resources.at(shaderUuid);

	if (res != nullptr)
	{
		this->shader = ShaderManager::ImportFromLibrary(res);
	}
}

#pragma endregion