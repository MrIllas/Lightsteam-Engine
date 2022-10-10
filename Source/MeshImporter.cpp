#include "MeshImporter.h"

#include "GLOBALS.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include "Mesh.h"

#pragma comment (lib, "assimp.lib")

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

std::vector<Mesh> MeshImporter::LoadMeshFile(std::string filePath)
{
	const aiScene* scene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	
	std::vector<Mesh> toReturn;

	if (scene != nullptr && scene->HasMeshes())
	{
		for(uint i = 0; i < scene->mNumMeshes; ++i)
		{
			 toReturn.emplace_back(LoadMesh(scene->mMeshes[i]));
		}

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", filePath.c_str());
	}

	return toReturn;
}

void MeshImporter::LoadNode(aiNode* node)
{

}

Mesh MeshImporter::LoadMesh(aiMesh* mesh)
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