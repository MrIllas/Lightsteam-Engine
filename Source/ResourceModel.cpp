#include "ResourceModel.h"

#include "MeshRenderer.h"

ResourceModel::ResourceModel(std::string uuid) : Resource(uuid, RESOURCE_TYPE::MODEL)
{
	meshRendererMap = new std::map<std::string, SubMeshResource*>();
}

ResourceModel::~ResourceModel()
{
	
}

void ResourceModel::CleanInstance()
{
	if (meshRendererMap != nullptr)
	{
		for (auto const& mesh : *meshRendererMap)
		{
			if (mesh.second != nullptr)
			{
				RELEASE(meshRendererMap->at(mesh.first)->meshRenderer);
				RELEASE(meshRendererMap->at(mesh.first));
			}
		}
	}
	
	meshRendererMap->clear(); //Not releasin SubMeshResource* only Mesh Renderer!!!
	RELEASE(meshRendererMap)
}

void ResourceModel::PlanDelete()
{
	if (meshRendererMap != nullptr)
	{
		for (auto const& mesh : *meshRendererMap)
		{
			if (mesh.second != nullptr)
			{
				meshRendererMap->at(mesh.first)->meshRenderer->planDelete = true;
			}
		}
	}
}

void ResourceModel::CleanMeshRendererMap()
{
	if (meshRendererMap != nullptr)
	{
		for (auto const& mesh : *meshRendererMap)
		{
			if (mesh.second != nullptr)
			{
				RELEASE(meshRendererMap->at(mesh.first)->meshRenderer);
				meshRendererMap->at(mesh.first)->referenceCount = 0;
			}
		}
	}
}

nlohmann::JsonData ResourceModel::SaveUnique(nlohmann::JsonData data)
{
	std::vector<nlohmann::ordered_json> aux;
	
	for (auto const& mesh : *meshRendererMap)
	{
		nlohmann::JsonData meshData;

		meshData.SetString("Uuid", mesh.first);
		meshData.SetString("Library Path", mesh.second->libPath);

		aux.emplace_back(meshData.data);
	}
	
	data.data.emplace("Model Meshes", aux);

	return data;
}

void ResourceModel::LoadUnique(nlohmann::JsonData data)
{
	std::vector<nlohmann::ordered_json> aux;

	aux = data.data["Model Meshes"].get<nlohmann::ordered_json>();

	if (meshRendererMap == nullptr) meshRendererMap = new std::map<std::string, SubMeshResource*>();

	if (aux.size() > 0)
	{
		for (int i = 0; i < aux.size(); ++i)
		{
			nlohmann::JsonData meshData;
			meshData.data = aux.at(i);

			SubMeshResource* subRes = new SubMeshResource();
			subRes->libPath = meshData.GetString("Library Path");


			meshRendererMap->emplace(meshData.GetString("Uuid"), subRes);
		}
	}
}