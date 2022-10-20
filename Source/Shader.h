#ifndef SHADER_H
#define SHADER_H

typedef unsigned int uint;


#include <string>

class Shader
{
public:
	uint ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void Use();

	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetMat4(const std::string& name, const float* value) const;
};

#endif