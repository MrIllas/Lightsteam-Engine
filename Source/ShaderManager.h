#pragma once

#include "Shader.h"
#include "ResourceShader.h"

#define BASE_SHADER_KEY "BASE SHADER"

namespace ShaderManager
{
	void Init();
	void Shutdown();

	bool ImportToLibrary(ResourceShader* resource);
	Shader* ImportFromLibrary(ResourceShader* resource);

	Shader* BaseShader();
	Shader* DebugShader();

	void HandleShaderGUI(ShaderUniform &uni);
};

