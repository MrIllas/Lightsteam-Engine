#pragma once

typedef unsigned int uint;
#include <string>
#include "JsonUtils.h"

class Shader;

class ShaderUniform
{
public:
	ShaderUniform();
	~ShaderUniform();

	void Update(Shader* shader);

	int read, size;
	uint type;
	std::string strType;
	std::string name;
	int index;
	void* value = nullptr;

public:


	void VariableSetting();
	void VariableDeleting();
	void HandleShaderGUI();

	nlohmann::ordered_json GetJSON();
	void SetJSON(nlohmann::JsonData data);

private:
	float time;
};

