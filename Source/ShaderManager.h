#pragma once

#include "Shader.h"
#include "ResourceShader.h"

namespace ShaderManager
{
	void Init();
	void Shutdown();

	bool ImportToLibrary(ResourceShader* resource);
	Shader* ImportFromLibrary(ResourceShader* resource);

	Shader* BaseShader();
	Shader* DebugShader();
};

