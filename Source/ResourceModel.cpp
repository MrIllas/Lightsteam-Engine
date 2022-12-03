#include "ResourceModel.h"

#include "MeshRenderer.h"

ResourceModel::ResourceModel(std::string uuid) : Resource(uuid, RESOURCE_TYPE::MODEL)
{
	meshRendererMap = new std::map<std::string, MeshRenderer*>();
	meshCCF = new std::map<std::string, std::string>();
}

ResourceModel::~ResourceModel()
{
	
}

void ResourceModel::CleanInstance()
{
	if (meshCCF != nullptr) meshCCF->clear();
	RELEASE(meshCCF);

	CleanMeshRendererMap();
}

void ResourceModel::CleanMeshRendererMap()
{
	if (meshRendererMap != nullptr)
	{
		//if (meshRendererMap->empty())return;

		for (auto const& mesh : *meshRendererMap)
		{
			if (mesh.second != nullptr)
			{
				RELEASE(meshRendererMap->at(mesh.first));
			}
		}

		meshRendererMap->clear();
	}
	
	RELEASE(meshRendererMap);
}

nlohmann::JsonData ResourceModel::SaveUnique(nlohmann::JsonData data)
{
	std::vector<nlohmann::ordered_json> aux;
	
	for (auto const& mesh : *meshCCF)
	{
		nlohmann::JsonData meshData;

		meshData.SetString("Uuid", mesh.first);
		meshData.SetString("Library Path", mesh.second);

		aux.emplace_back(meshData.data);
	}
	
	data.data.emplace("Model Meshes", aux);

	return data;
}

void ResourceModel::LoadUnique(nlohmann::JsonData data)
{
	std::vector<nlohmann::ordered_json> aux;

	aux = data.data["Model Meshes"].get<nlohmann::ordered_json>();

	if (aux.size() > 0)
	{
		for (int i = 0; i < aux.size(); ++i)
		{
			nlohmann::JsonData meshData;
			meshData.data = aux.at(i);

			meshCCF->emplace(meshData.GetString("Uuid"), meshData.GetString("Library Path"));
		}
	}
}