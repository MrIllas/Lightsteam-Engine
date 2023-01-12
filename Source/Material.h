#pragma once

#include "JsonUtils.h"

#include <string>
#include <vector>
#
class Shader;
class ShaderUniform;
class ResourceTexture;

class Material
{
public:
	Material(std::string name = "Material");
	~Material();

	Shader* GetShader() { return shader; }
	void SetShader(Shader* shader);
	void SetDefaultShader(ResourceTexture* texture);

	void Save(std::string filepath);
	void Load(nlohmann::JsonData data);

private:
	void CleanUniforms();

public:
	std::string uuid;
	std::string name;

	std::vector<ShaderUniform*> uniforms;
private:
	Shader* shader = nullptr;

	
};

