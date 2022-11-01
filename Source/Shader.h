#ifndef SHADER_H
#define SHADER_H

typedef unsigned int uint;

#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath, std::string name = "Shader");

	void Use();

	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetMat4(const std::string& name, const float* value) const;
	void SetVec3(const std::string& name, const float* value) const;

private:
	void RetriveShader(const char* vertexPath, const char* fragmentPath);
	void CompileShader();

public:
	uint ID;
	std::string name;
private:
	std::string vertexCode;
	std::string fragmentCode;
};

#endif