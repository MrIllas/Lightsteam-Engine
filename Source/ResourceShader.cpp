#include "ResourceShader.h"

#include "ShaderManager.h"
#include "ModuleEditor.h"
#include "Material.h"

ResourceShader::ResourceShader(std::string uuid) : Resource(uuid, RESOURCE_TYPE::SHADER)
{
}

ResourceShader::~ResourceShader()
{
}

void ResourceShader::CleanInstance()
{
	if (matRef != nullptr)
	{
		matRef->clear();
		RELEASE(matRef);
	}

	if (shader != nullptr) RELEASE(shader);
}

void ResourceShader::PlanDelete()
{
	if (isOpenOnEditor)
	{
		EditorProperties::Instance()->ForceClose();
	}

	if (matRef == nullptr) return;
	for (int i = 0; i < matRef->size(); ++i)
	{
		matRef->at(i)->SetShader(nullptr);
		matRef->at(i) = nullptr;
	}

	matRef->clear();
}

void ResourceShader::SetShaderToMat(Material* mat)
{
	mat->SetShader(ShaderManager::ImportFromLibrary(this));
	IncreaseRC();

	if (matRef == nullptr) matRef = new std::vector<Material*>();

	matRef->push_back(mat);
}

void ResourceShader::RemoveShaderFromMat(Material* mat)
{
	auto it = std::find(matRef->begin(), matRef->end(), mat);

	if (it != matRef->end())
	{
		matRef->erase(it);
	}

	DecreaseRC();
	if (shader == nullptr) LOG(LOG_TYPE::ATTENTION, "RC 0: Unloading shader '%s' from memory!", GetLibraryFile().c_str());
}

void ResourceShader::SetShader(Shader* shader)
{
	this->shader = shader; 
	shader->uuid = uuid;
}

#pragma region Save&Load
nlohmann::JsonData ResourceShader::SaveUnique(nlohmann::JsonData data)
{
	data.SetInt("Binary Format", this->binaryFormat);
	return data;
}

void ResourceShader::LoadUnique(nlohmann::JsonData data)
{
	this->binaryFormat = data.GetInt("Binary Format");
}
#pragma endregion