#pragma once

typedef unsigned int uint;
#include <string>
#include "JsonUtils.h"


class ShaderUniform
{
public:
	ShaderUniform();
	~ShaderUniform();


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
};

