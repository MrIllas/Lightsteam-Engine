#ifndef SHADER_H
#define SHADER_H

typedef unsigned int uint;

#include <string>

class Shader
{
public:
	Shader(char* buffer, uint size, uint format, std::string name = "Shader");
	Shader(std::string fragmentCode, std::string vertexCode, std::string name = "Shader");
	Shader(const char* shaderPath, std::string name = "Shader");
	~Shader();


	void Use();
	std::string GetBinary();

	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetMat4(const std::string& name, const float* value) const;
	void SetVec3(const std::string& name, const float* value) const;
	void SetVec2(const std::string& name, const float* value) const;

private:
	void RetriveShader(const char* shaderPath);
	void CompileShader();
	void LoadBinary(char* buffer, uint size, uint format);

public:
	uint ID;
	std::string uuid;
	std::string name;
private:
	std::string vertexCode;
	std::string fragmentCode;
};

#endif