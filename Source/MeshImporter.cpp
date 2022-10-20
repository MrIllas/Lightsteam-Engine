#include "MeshImporter.h"

#include "GLOBALS.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "assimp.lib")

#include "GameObject.h"
#include "CompMeshRenderer.h"

#include"ModuleScene.h"

MeshImporter::MeshImporter()
{

}

MeshImporter::~MeshImporter()
{

}

void MeshImporter::Start()
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

void MeshImporter::ImportMesh(std::string filePath, bool dragAndDrop)
{
	//ASK - UTF8 Characters not accepted by ASSIMP on the current lib version(2016) but added in 2017 https://github.com/assimp/assimp/issues/1612

	const aiScene* scene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs);
	aiNode* node = nullptr;
	if (scene != nullptr && scene->HasMeshes())
	{
		if (dragAndDrop) LOG(LOG_TYPE::SUCCESS, "IMPORTING FILE: %s", filePath.c_str());

		node = scene->mRootNode;

		SceneProperties::Instance()->root->AddChildren(GenerateGameObjects(node, scene));
	}

	if(scene == nullptr && dragAndDrop) LOG(LOG_TYPE::ERRO, "ERROR: Importing file: '%s'", filePath.c_str());
}

GameObject* MeshImporter::GenerateGameObjects(aiNode* node, const aiScene* scene, GameObject* parent)
{

	if(parent == nullptr) parent = new GameObject(scene->mRootNode->mName.C_Str());

	if (scene->HasMeshes())
	{
		/*NOMES L'ULTIMA MESH S'ESTA VISUALITZANT*/

		//Meshes
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			//New Spatial GameObject with MeshRenderer component
			GameObject* go = new GameObject(node->mChildren[i]->mName.C_Str());
			go->CreateComponent(MESH_RENDERER);

			//Add Mesh to the gameObject
			MeshRenderer* meshRenderer = new MeshRenderer(GenerateMesh(scene->mMeshes[i]));
			go->GetComponent<CompMeshRenderer>(MESH_RENDERER)->SetMesh(meshRenderer);

			//Recursivnes
			for (uint i2 = 0; i2 < node->mChildren[i]->mNumChildren; ++i2)
			{
				go = GenerateGameObjects(node->mChildren[i]->mChildren[i2], scene, go);
			}

			//Add GameObject to it's parent
			parent->AddChildren(go);
		}
	}

	return parent;
}

Meshe MeshImporter::GenerateMesh(aiMesh* mesh)
{
	Meshe newMesh = Meshe();
	//newMesh.InitMesh();

	bool hasTex = mesh->HasTextureCoords(0);

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
	LOG(LOG_TYPE::SUCCESS, "New mesh with %d vertices", mesh->mNumVertices);

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
	return newMesh;
}