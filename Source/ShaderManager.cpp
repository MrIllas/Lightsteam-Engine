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

void ShaderManager::GenerateBinary(uint programID, std::string uuid)
{
	GLint size = 0;
	GLenum format = 0;
	std::vector<GLubyte> buffer(size);
	
	//Get Binary code
	glGetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &size);
	glGetProgramBinary(programID, size, NULL, &format, buffer.data());

	//Write to a file
	std::string filePath = LIB_SHADERS;
	filePath += uuid;
	filePath += ".lssbin";

	LibraryManager::Save(filePath, reinterpret_cast<char*>(buffer.data()), size);

}

void ShaderManager::ImportToLibrary(ResourceShader* resource)
{

}