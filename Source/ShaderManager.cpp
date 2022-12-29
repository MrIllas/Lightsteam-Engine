#include "ShaderManager.h"

#include "LibraryManager.h"

#include "BaseShaders.h"

#include "Glew/include/glew.h"

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
	if (resource->shader == nullptr)
	{
		//Get name
		resource->SetName(LibraryManager::GetFilename(resource->GetAssetsFile()));

		//Compile Shader
		resource->shader = new Shader(resource->GetAssetsFile().c_str(), resource->GetName());
	}

	//Generate and save binary
	GLint size = 0;
	GLenum format = 0;

	//Binary code
	glGetProgramiv(resource->shader->ID, GL_PROGRAM_BINARY_LENGTH, &size);
	std::vector<GLubyte> buffer(size);
	glGetProgramBinary(resource->shader->ID, size, NULL, &format, buffer.data());

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
	resource->SetName(resource->shader->name);

	RELEASE(resource->shader);

	return isError;
}

Shader* ShaderManager::ImportFromLibrary(ResourceShader* resource)
{
	//Checks if the binary exists, imports it first if it doesn't
	if (!LibraryManager::Exists(resource->GetLibraryFile())) ImportToLibrary(resource);

	char* buffer = nullptr;
	GLint size = 0;

	size = LibraryManager::Load(resource->GetLibraryFile(), &buffer);

	if (resource->shader != nullptr) RELEASE(resource->shader); //Clean current shader(if there is one)
	resource->shader = new Shader(buffer, size, resource->binaryFormat, resource->GetName());
	resource->shader->uuid = resource->GetUUID();

	RELEASE(buffer);

	return resource->shader;
}