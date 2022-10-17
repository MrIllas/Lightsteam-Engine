#include "MeshImporter.h"

#include "GLOBALS.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "assimp.lib")

#include "Mesh.h"

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

void MeshImporter::ImportMesh(std::string filePath)
{
	const aiScene* scene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	aiNode* node = nullptr;
	if (scene != nullptr && scene->HasMeshes())
	{
		node = scene->mRootNode;

		SceneProperties::Instance()->root->AddChildren(GenerateGameObjects(node, scene));
	}
}

//void MeshImporter::LoadMeshFile(std::string filePath)
//{
//	const aiScene* scene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
//
//	if (scene != nullptr && scene->HasMeshes())
//	{
//		for (uint i = 0; i < scene->mNumMeshes; ++i)
//		{
//			/*CHECK THIS - NOT WORKING, SHOULD LOAD MESHES; CREATE THE COMP ANDD ADD IT TO THE SCENE*/
//			GameObject* go = new GameObject("nodeName");
//			go->CreateComponent(MESH_RENDERER);
//
//			MeshRenderer* meshRenderer = new MeshRenderer(LoadMesh(scene->mMeshes[i]));
//
//			go->GetComponent<CompMeshRenderer>(MESH_RENDERER)->SetMesh(meshRenderer);
//			SceneProperties::Instance()->root->AddChildren(go);
//		}
//
//		aiReleaseImport(scene);
//	}
//	else
//	{
//		LOG("Error loading scene %s", filePath.c_str());
//	}
//}

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

Mesh MeshImporter::GenerateMesh(aiMesh* mesh)
{
	Mesh newMesh = Mesh();
	newMesh.InitMesh();

	//Vertex
	for (uint i = 0; i < mesh->mNumVertices; ++i)
	{
		newMesh.vertex.emplace_back(float3( mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
	}
	LOG("New mesh with %d vertices", mesh->mNumVertices);

	//Index
	if (mesh->HasFaces())
	{
		newMesh.index.resize(mesh->mNumFaces * 3);
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3) 
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&newMesh.index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}
	return newMesh;
}