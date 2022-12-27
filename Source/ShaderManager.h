#pragma once

#include "Shader.h"
#include "ResourceShader.h"

namespace ShaderManager
{
	void Init();
	void Shutdown();

	void GenerateBinary(uint programID, std::string uuid);
	void ImportToLibrary(ResourceShader* resource);

	Shader* BaseShader();
	Shader* DebugShader();
};

