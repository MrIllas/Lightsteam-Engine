#include "ShaderManager.h"

#include "LibraryManager.h"

#include "BaseShaders.h"

#include "Glew/include/glew.h"
#include "ImGui/imgui.h"

#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float4x4.h"

#include "TextureImporter.h"
#include "LibraryFolder.h"
#include "ModuleResources.h"
#include "ResourceTexture.h"

#include "LSUUID.h"

static Shader* baseShader;
Shader* ShaderManager::BaseShader() { return baseShader; }

static Shader* debugShader;
Shader* ShaderManager::DebugShader() { return debugShader; }

void ShaderManager::Init()
{
	LOG(LOG_TYPE::ATTENTION, "Compiling In-Engine shaders.");

	baseShader = new Shader(InEngineShaders::baseFragment,
		InEngineShaders::baseVertex,
		"Lighsteam basic");

	debugShader = new Shader(InEngineShaders::debugFragment,
		InEngineShaders::debugVertex,
		"Lightsteam debug");

}

void ShaderManager::Shutdown()
{
	if (baseShader != nullptr) RELEASE(baseShader);
	if (debugShader != nullptr) RELEASE(debugShader);
}

bool ShaderManager::ImportToLibrary(ResourceShader* resource)
{
	bool isError = false;

	Shader* shader = resource->GetShader();

	if (shader == nullptr)
	{
		//Get name
		resource->SetName(LibraryManager::GetFilename(resource->GetAssetsFile()));

		//Compile Shader
		shader = new Shader(resource->GetAssetsFile().c_str(), resource->GetName());
		//resource->SetShader(shader);
	}
	else shader->Recompile(resource->GetAssetsFile().c_str(), resource->GetName());

	//Generate and save binary
	GLint size = 0;
	GLenum format = 0;

	//Binary code
	glGetProgramiv(shader->ID, GL_PROGRAM_BINARY_LENGTH, &size);
	std::vector<GLubyte> buffer(size);
	glGetProgramBinary(shader->ID, size, NULL, &format, buffer.data());

	//Write to a library file
	std::string filePath = LIB_SHADERS;
	filePath += "/";
	filePath += resource->GetUUID();
	filePath += ".lssbin";

	if(buffer.size() != 0) 
		LibraryManager::Save(filePath, reinterpret_cast<char*>(buffer.data()), size);
	else 
		isError = true;

	resource->SetLibraryFile(filePath);
	resource->binaryFormat = format;
	resource->SetName(shader->name);
	resource->SetVersion(LS_UUID::Generate());

	if(resource->GetRC() < 1) RELEASE(shader);

	return isError;
}

Shader* ShaderManager::ImportFromLibrary(ResourceShader* resource)
{
	//Checks if the binary exists, imports it first if it doesn't
	if (!LibraryManager::Exists(resource->GetLibraryFile())) ImportToLibrary(resource);

	char* buffer = nullptr;
	GLint size = 0;

	size = LibraryManager::Load(resource->GetLibraryFile(), &buffer);

	Shader* shader = resource->GetShader();

	if (shader != nullptr) RELEASE(shader); //Clean current shader(if there is one)
	shader = new Shader(buffer, size, resource->binaryFormat, resource->GetName());
	
	resource->SetShader(shader);
	RELEASE(buffer);

	return shader;
}