#pragma once

#include "JsonUtils.h"

#include <string>
#include <vector>
#
class Shader;
class ResourceTexture;

class Material
{
public:
	Material(std::string name = "Material");
	~Material();

	Shader* GetShader() { return shader; }
	void SetShader(Shader* shader) { this->shader = shader; }
	void SetDefaultShader(ResourceTexture* texture);

	void Save(std::string filepath);
	void Load(nlohmann::JsonData data);

public:
	std::string uuid;
	std::string name;
private:
	Shader* shader = nullptr;
};

