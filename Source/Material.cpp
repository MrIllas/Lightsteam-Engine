#include "Material.h"

#include "ModuleResources.h"
#include "ResourceMaterial.h"
#include "ResourceShader.h"
#include "ShaderManager.h"

#include "LibraryManager.h"

#include "Shader.h"
#include "ShaderUniform.h"
#include "ResourceTexture.h"

Material::Material(std::string name)
{
	this->name = name;
}

Material::~Material()
{
	if (this->shader != nullptr)
	{
		if (this->shader->uuid.empty() || this->shader->uuid == "")
		{ //Base Shader
		}
		else
		{
			ResourceProperties::Instance()->resources.at(shader->uuid)->DecreaseRC();
		}	
	}

	CleanUniforms();
}

void Material::CleanUniforms()
{
	for (int i = 0; i < uniforms.size(); ++i)
	{
		RELEASE(uniforms[i]);
	}
	uniforms.clear();
}

void Material::SetDefaultShader(ResourceTexture* resource)
{
	this->shader = ShaderManager::BaseShader();
	CleanUniforms();
	this->shader->VariableParser(this->uniforms);
	
	for (int i = 0; i < uniforms.size(); ++i)
	{
		if (uniforms[i]->isTexture() && resource != nullptr)
		{
			uniforms[i]->VariableDeleting();
			uniforms[i]->value = resource->texture;
		}
	}
}

void Material::SetShader(Shader* shader)
{
	this->shader = shader;
	CleanUniforms();

	if(this->shader != nullptr) shader->VariableParser(this->uniforms);
}

#pragma region Save&Load

void Material::Save(std::string filepath)
{
	nlohmann::JsonData data;
	
	data.SetString("Name", this->name);
	data.SetString("UUID", this->uuid);

	if (shader != nullptr)
	{
		data.SetString("Shader Uuid", shader->uuid);

		if(shader->uuid.empty())
		{
			data.SetString("Shader Compile Version", BASE_SHADER_KEY);
		}
		else
			data.SetString("Shader Compile Version", ResourceProperties::Instance()->resources.at(shader->uuid)->GetVersion());

		std::vector<nlohmann::ordered_json> aux;
		//Save uniforms variables
		for (int i = 0; i < uniforms.size(); ++i)
		{
			aux.push_back(uniforms[i]->GetJSON());
		}
		data.data.emplace("Uniforms", aux);
	}

	LibraryManager::SaveJSON(filepath, data.data.dump(4));
}

void Material::Load(nlohmann::JsonData data)
{
	this->name = data.GetString("Name");
	this->uuid = data.GetString("UUID");

	//Load Shader
	std::string shaderUuid = data.GetString("Shader Uuid");

	if (shaderUuid == "NULL" || shaderUuid.empty()) this->shader = ShaderManager::BaseShader();

	ResourceShader* res = nullptr;
	if (ResourceProperties::Instance()->resources.count(shaderUuid) == 1)
		res = (ResourceShader*)ResourceProperties::Instance()->resources.at(shaderUuid);

	if (res != nullptr)
	{
		this->shader = ShaderManager::ImportFromLibrary(res);

		if (this->shader == nullptr) this->shader = ShaderManager::BaseShader();

		//Increase reference count
		res->IncreaseRC();
	}
	
	//If same version load uniforms
	if (( res != nullptr && res->GetVersion() == data.GetString("Shader Compile Version")) || data.GetString("Shader Compile Version") == BASE_SHADER_KEY)
	{
		std::vector<nlohmann::ordered_json> aux;

		aux = data.data["Uniforms"].get<nlohmann::ordered_json>();

		if (aux.size() > 0)
		{
			CleanUniforms();
			this->shader->VariableParser(this->uniforms);
			for (int i = 0; i < aux.size(); ++i)
			{
				nlohmann::JsonData uniData;
				uniData.data = aux.at(i);

				uniforms[i]->SetJSON(uniData);
			}
		}
	}
}

#pragma endregion