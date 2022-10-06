#include "AssetImporter.h"

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

