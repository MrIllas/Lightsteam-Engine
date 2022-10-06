#include "AssetImporter.h"

#include "GLOBALS.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "assimp.lib")

AssetImporter::AssetImporter()
{

}

AssetImporter::~AssetImporter()
{

}

void AssetImporter::Start()
{
	//Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void AssetImporter::CleanUp()
{
	//detach log stream
	aiDetachAllLogStreams();
}

void AssetImporter::LoadFile(std::string filePath)
{
	const aiScene* scene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", filePath.c_str());
	}
}
		