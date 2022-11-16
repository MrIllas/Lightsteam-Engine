#include "MeshImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "assimp.lib")

#include "GameObject.h"

#include "CompMeshRenderer.h"

#include "ModuleScene.h"
#include "LibraryManager.h"

#include <vector>


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

GameObject* MeshImporter::ImportMesh(std::string filePath, GameObject* parent, bool dragAndDrop)
{
	GameObject* toReturn = nullptr;
	//ASK - UTF8 Characters not accepted by ASSIMP on the current lib version(2016) but added in 2017 https://github.com/assimp/assimp/issues/1612

	const aiScene* scene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
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

	return toReturn;
}

GameObject* MeshImporter::GenerateGameObjects(aiNode* node, const aiScene* scene, GameObject* parent)
{

	if(parent == nullptr && scene->mNumMeshes > 1) parent = new GameObject(scene->mRootNode->mName.C_Str());

	if (scene->HasMeshes())
	{
		//Meshes
		for (uint i = 0; i < node->mNumChildren; ++i)
		{
			//New Spatial GameObject with MeshRenderer component
			GameObject* go = new GameObject(node->mChildren[i]->mName.C_Str());
			go->CreateComponent(MESH_RENDERER);
			go->CreateComponent(MATERIAL);

			//Add Mesh to the gameObject
			Meshe mesh;

			std::string aux = "Library/Meshes/";
			aux += std::string(node->mChildren[i]->mName.C_Str());
			aux += ".mh";

			//Checks if the mesh already exists in the engine's CFF
			if (LibraryManager::Exists(aux))
			{
				mesh = LoadMesh(aux);
			}
			else
			{
				mesh = GenerateMesh(scene->mMeshes[i]);
				SaveMesh(mesh, aux);
			}
			mesh.path = aux;

			MeshRenderer* meshRenderer = new MeshRenderer(mesh);
			go->GetComponent<CompMeshRenderer>(MESH_RENDERER)->SetMesh(meshRenderer);

			//Recursivnes
			if (node->mChildren[i]->mNumChildren > 1)
			{
				for (uint i2 = 0; i2 < node->mChildren[i]->mNumChildren; ++i2)
				{
					go = GenerateGameObjects(node->mChildren[i]->mChildren[i2], scene, go);
				}
			}

			//Add GameObject to it's parent
			if (parent == nullptr) parent = go;
			else parent->AddChildren(go);
		}
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

	return newMesh;
}

void MeshImporter::SaveMesh(Meshe mesh, std::string filePath)
{
	uint ranges[3] =
	{
		mesh.indices.size(),
		mesh.vertices.size(),
		mesh.numFaces
	};

	uint size = sizeof(ranges) + sizeof(uint) * ranges[0] + sizeof(Vertex) * ranges[1] + sizeof(uint) * ranges[2];

	
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

	LibraryManager::Save(filePath, fileBuffer, size);

	RELEASE_ARRAY(fileBuffer);
}

Meshe MeshImporter::LoadMesh(std::string filePath)
{
	Meshe mesh;
	char* fileBuffer = nullptr;
	LibraryManager::Load(filePath, &fileBuffer);

	char* cursor = fileBuffer;

	uint ranges[3];
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

	//Store number of faces
	bytes = sizeof(uint);
	memcpy(&mesh.numFaces, cursor, bytes);
	cursor += bytes;

	RELEASE_ARRAY(fileBuffer);

	return mesh;
}