#include "MeshImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "assimp.lib")

#include "GameObject.h"

#include "CompTransform.h"
#include "CompMeshRenderer.h"

#include "ModuleScene.h"
#include "LibraryManager.h"
#include "ResourceModel.h"
#include "LSUUID.h"


MeshImporter::MeshImporter()
{

}

MeshImporter::~MeshImporter()
{

}

void MeshImporter::Init()
{
	//Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void MeshImporter::CleanUp()
{
	//detach log stream
	aiDetachAllLogStreams();
}

///DEPRECATED
GameObject* MeshImporter::ImportMesh(std::string filePath, GameObject* parent, bool dragAndDrop)
{
	GameObject* toReturn = nullptr;

	const aiScene* scene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_Fast);
	aiNode* node = nullptr;
	if (scene != nullptr && scene->HasMeshes())
	{
		if (dragAndDrop) LOG(LOG_TYPE::SUCCESS, "IMPORTING FILE: %s", filePath.c_str());

		node = scene->mRootNode;

		if (parent == nullptr)
		{
			toReturn = GenerateGameObjects(node, scene);
			SceneProperties::Instance()->root->AddChildren(toReturn);
		}
		else
		{
			parent->AddChildren(GenerateGameObjects(node, scene));
			toReturn = parent;
		}
	}

	if(scene == nullptr && dragAndDrop) LOG(LOG_TYPE::ERRO, "ERROR: Importing file: '%s'", filePath.c_str());

	//Save model
	/*if (toReturn != nullptr)
	{
		std::vector<nlohmann::ordered_json> goPool;
		SaveGameObjects(toReturn go);
	}*/

	return toReturn;
}

MeshRenderer* MeshImporter::ImportMeshFromLibrary(ResourceModel* model, std::string meshUuid)
{
	MeshRenderer* toReturn = nullptr;
	SubMeshResource* subMesh = model->meshRendererMap->at(meshUuid);
	if (subMesh != nullptr)
	{
		subMesh->referenceCount++;

		//Id the MeshRenderer == nullptr means that the model is not loaded
		if (subMesh->meshRenderer == nullptr)
		{
			subMesh->meshRenderer = new MeshRenderer(MeshImporter::LoadMesh(subMesh->libPath));
			LOG(LOG_TYPE::ATTENTION, "Loading new mesh to memory.");
		}
		else LOG(LOG_TYPE::SUCCESS, "This mesh is already loaded. Current copies: %i", subMesh->referenceCount);
			

		toReturn = subMesh->meshRenderer;

	}

	return toReturn;
}

GameObject* MeshImporter::ImportFromLibrary(ResourceModel* resource)
{
	GameObject* toReturn = new GameObject("MODEL IMPORT", false);
	nlohmann::JsonData data;

	try
	{
		char* buffer = nullptr;

		uint size = LibraryManager::Load(resource->GetLibraryFile(), &buffer);
		data.data = nlohmann::ordered_json::parse(buffer, buffer + size);
		RELEASE(buffer);
	}
	catch (nlohmann::json::parse_error& ex)
	{
		LOG(LOG_TYPE::ERRO, "Error: Model parse at byte %i: %s", ex.byte, ex.what());
		return nullptr;
	}

	
	//Load model root
	std::map<std::string, GameObject*> modelMap;
	std::vector<nlohmann::ordered_json> aux;

	nlohmann::JsonData rootData;
	aux = data.GetJsonVector("Model");

	rootData.data = aux.at(0);
	toReturn->Load(rootData);

	//Load all other GO
	for (int i = 1; i < aux.size(); ++i)
	{
		nlohmann::JsonData goData;
		goData.data = aux.at(i);

		std::string parentUUID(goData.GetString("Parent_UUID"));

		GameObject* go = new GameObject("", false);
		go->Load(goData);

		if (modelMap.count(parentUUID) == 1)
		{
			modelMap[parentUUID]->AddChildren(go);
		}
		else toReturn->AddChildren(go);


		modelMap.insert({ go->uuid, go });
	}

	return toReturn;
}

void MeshImporter::ImportToLibrary(ResourceModel* resource)
{
	GameObject* parent = nullptr;

	const aiScene* scene = aiImportFile(resource->GetAssetsFile().c_str(), aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure | aiProcess_FindInstances);
	aiNode* node = nullptr;

	//Load model from assets
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG(LOG_TYPE::SUCCESS, "IMPORTING MODEL TO LIB: %s", resource->GetAssetsFile().c_str());

		node = scene->mRootNode;

		parent = GenerateGameObjects(node, scene, nullptr, resource);
	}
	

	if (parent != nullptr)
	{
		//Save model to Library/Model
		nlohmann::JsonData data;
		std::vector<nlohmann::ordered_json> goPool;

		SaveGameObjects(parent, goPool);
		data.data.emplace("Model", goPool);

		std::string savePath = LIB_MODELS;
		savePath += "/";
		savePath += LS_UUID::Generate();
		savePath += ".mdl";

		LibraryManager::SaveJSON(savePath, data.data.dump(4));
		resource->SetLibraryFile(savePath);	
	}

	//Unloads anything that has been loaded for Library import reasons.
	resource->CleanMeshRendererMap();

	RELEASE(parent);
}

GameObject* MeshImporter::GenerateGameObjects(aiNode* node, const aiScene* scene, GameObject* parent, ResourceModel* resource)
{
	bool parentNoMesh = false;
	//if(parent == nullptr && scene->mNumMeshes > 1) parent = new GameObject(scene->mRootNode->mName.C_Str());
	
	GameObject* go = new GameObject(node->mName.C_Str());

	//GET SET transform
	aiMatrix4x4 aiTransform = node->mTransformation;
	float4x4 fTransform = float4x4(aiTransform[0][0], aiTransform[0][1], aiTransform[0][2], aiTransform[0][3],
		aiTransform[1][0], aiTransform[1][1], aiTransform[1][2], aiTransform[1][3],
		aiTransform[2][0], aiTransform[2][1], aiTransform[2][2], aiTransform[2][3],
		aiTransform[3][0], aiTransform[3][1], aiTransform[3][2], aiTransform[3][3]
	);
	go->GetComponent<CompTransform>(TRANSFORM)->SetWorldMatrix(fTransform);

	if (node->mNumMeshes >= 1)
	{
		for (uint i = 0; i < node->mNumMeshes; ++i)
		{
			//New Spatial GameObject with MeshRenderer component
			aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];

			GameObject* meshGo = nullptr;

			if (node->mNumMeshes == 1) meshGo = go;
			else meshGo = new GameObject(aimesh->mName.C_Str());

			meshGo->CreateComponent(MESH_RENDERER);
			meshGo->CreateComponent(MATERIAL);

			Meshe mesh;

			//Checks if the mesh already exists in the engine's CFF
			std::string uuid = LS_UUID::Generate();
			std::string aux = LIB_MESHES;
			aux += "/";
			aux += uuid;
			aux += ".mh";

			mesh = GenerateMesh(aimesh);
			SaveMesh(mesh, aux);
			mesh.path = aux;

			MeshRenderer* meshRenderer = new MeshRenderer(mesh);
			meshGo->GetComponent<CompMeshRenderer>(MESH_RENDERER)->SetMesh(meshRenderer);

			meshRenderer->uuid = uuid;
			meshRenderer->modelUuid = resource->GetUUID();

			//Store mesh in ModelResource
			if (resource != nullptr) {
				SubMeshResource* subRes = new SubMeshResource();
				subRes->libPath = aux;
				subRes->referenceCount++;
				subRes->meshRenderer = meshRenderer;
				//resource->meshRendererMap->emplace(uuid, meshRenderer);
				resource->meshRendererMap->emplace(uuid, subRes);
				//resource->meshCCF->emplace(uuid, aux);
			}
			if (node->mNumMeshes != 1) go->AddChildren(meshGo);
		}
	}

	//Recursivnes
	if (node->mNumChildren >= 1)
	{
		for (uint k = 0; k < node->mNumChildren; ++k)
		{
			GenerateGameObjects(node->mChildren[k], scene, go, resource);
		}
	}

	//Add GameObject to it's parent
	if (parent == nullptr) parent = go;
	//else if (parent->GetComponent<MeshRenderer>(MESH_RENDERER) == nullptr)
	//{
	//	//parent = go;
	//}
	else
	{
		parent->AddChildren(go);
	}

	return parent;
}

Meshe MeshImporter::GenerateMesh(aiMesh* mesh)
{
	Meshe newMesh = Meshe();
	//newMesh.InitMesh();

	bool hasTex = mesh->HasTextureCoords(0);

	newMesh.numFaces = mesh->mNumFaces;

	//Vertex
	for (uint i = 0; i < mesh->mNumVertices; ++i)
	{
		float3 position = float3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		float3 normal; 
		float2 textCoords = float2(0.0f, 0.0f);

		if (mesh->HasNormals()) normal = float3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (hasTex) textCoords = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

		newMesh.vertices.emplace_back(position, normal, textCoords);

	}

	//Index
	if (mesh->HasFaces())
	{
		newMesh.indices.resize(mesh->mNumFaces * 3);
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3) 
			{
				LOG(LOG_TYPE::ATTENTION, "WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&newMesh.indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	//Generate AABB
	newMesh.localAABB.SetNegativeInfinity();
	newMesh.localAABB.Enclose((float3 *)mesh->mVertices, newMesh.vertices.size());

	return newMesh;
}

void MeshImporter::SaveMesh(Meshe mesh, std::string filePath)
{
	uint ranges[2] =
	{
		mesh.indices.size(),
		mesh.vertices.size(),
	};

	uint size = sizeof(ranges) + sizeof(uint) * ranges[0] + sizeof(Vertex) * ranges[1] + sizeof(uint) + sizeof(float3) * 2;

	
	char* fileBuffer = new char[size];//Allocate
	char* cursor = fileBuffer;

	uint bytes = 0;

	//Store ranges
	bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	//Store indices
	bytes = sizeof(uint) * ranges[0];
	memcpy(cursor, &mesh.indices[0], bytes);
	
	cursor += bytes;

	//Store vertex position
	bytes = sizeof(Vertex) * ranges[1];
	memcpy(cursor, &mesh.vertices[0], bytes);
	cursor += bytes;

	//Store number of faces
	bytes = sizeof(uint);
	memcpy(cursor, &mesh.numFaces, bytes);
	cursor += bytes;

	//Bounding Box
	//Store BoundingBox MinPoint
	bytes = sizeof(float3);
	memcpy(cursor, &mesh.localAABB.minPoint[0], bytes);
	cursor += bytes;

	//Store BoundingBox MaxPoint
	bytes = sizeof(float3);
	memcpy(cursor, &mesh.localAABB.maxPoint[0], bytes);
	cursor += bytes;

	LibraryManager::Save(filePath, fileBuffer, size);

	RELEASE_ARRAY(fileBuffer);
}

Meshe MeshImporter::LoadMesh(std::string filePath)
{
	Meshe mesh;
	char* fileBuffer = nullptr;
	LibraryManager::Load(filePath, &fileBuffer);
	mesh.path = filePath;

	char* cursor = fileBuffer;

	uint ranges[2];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	mesh.indices.resize(ranges[0]);
	mesh.vertices.resize(ranges[1]);
	mesh.numFaces = 0;

	//Load indices
	bytes = sizeof(uint) * ranges[0];

	memcpy(&mesh.indices[0], cursor, bytes);
	cursor += bytes;

	//Load vertices
	bytes = sizeof(Vertex) * ranges[1];

	memcpy(&mesh.vertices[0], cursor, bytes);
	cursor += bytes;

	//Load number of faces
	bytes = sizeof(uint);
	memcpy(&mesh.numFaces, cursor, bytes);
	cursor += bytes;

	//Bounding Box
	mesh.localAABB.SetNegativeInfinity();
	//Load BoundingBox MinPoint
	bytes = sizeof(float3);
	memcpy(&mesh.localAABB.minPoint[0], cursor, bytes);
	cursor += bytes;

	//Load BoundingBox MaxPoint
	bytes = sizeof(float3);
	memcpy(&mesh.localAABB.maxPoint[0], cursor, bytes);
	cursor += bytes;

	RELEASE_ARRAY(fileBuffer);

	return mesh;
}

void MeshImporter::SaveGameObjects(GameObject* go, std::vector<nlohmann::ordered_json>& goPool)
{
	goPool.push_back(go->Save());

	if (go->HasChildren())
	{
		for (int i = 0; i < go->children.size(); ++i)
		{
			SaveGameObjects(go->children[i], goPool);
		}
	}
}