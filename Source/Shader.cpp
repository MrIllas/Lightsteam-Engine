#include "Shader.h"

#include "Glew/include/glew.h"

#include "Globals.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>

/// <summary>
/// Constructor used fro shaders that are being compiled from a code std::string variable.
/// </summary>
/// <param name="fragmentCode"></param>
/// <param name="vertexCode"></param>
/// <param name="name"></param>
Shader::Shader(std::string fragmentCode, std::string vertexCode, std::string name)
{
	this->name = name;

	this->fragmentCode = fragmentCode;
	this->vertexCode = vertexCode;

	CompileShader();
}

/// <summary>
/// Constructor used for shaders that are being loaded and compiled from an external file.
/// </summary>
/// <param name="vertexPath"></param>
/// <param name="fragmentPath"></param>
/// <param name="name"></param>
Shader::Shader(const char* shaderPath, std::string name)
{
	this->name = name;

	RetriveShader(shaderPath);
	CompileShader();
}

void Shader::RetriveShader(const char* shaderPath)
{
	const char* vDefine = "#define VERTEX_PROGRAM\n";
	const char* fDefine = "#define FRAGMENT_PROGRAM\n";

	std::ifstream shaderFile;

	//Ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//Open File
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		//Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		//Close file handlers
		shaderFile.close();

		//Convert stream into string
		std::string fullFile = shaderStream.str();

		//Gets version line
		std::string version = fullFile.substr(0, fullFile.find("\n")+1);
		std::size_t i = fullFile.find(version);
		fullFile.erase(i, version.length());

		//Code
		vertexCode = version + vDefine + fullFile;
		fragmentCode = version + fDefine + fullFile;
	}
	catch (std::ifstream::failure e)
	{
		LOG(LOG_TYPE::ERRO, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
}

void Shader::CompileShader()
{
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	uint vertex, fragment;
	int success;
	char infoLog[512];

	//Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//Print compiler errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		LOG(LOG_TYPE::ERRO, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s", infoLog);
	}

	//Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//Print compiler errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		LOG(LOG_TYPE::ERRO, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s", infoLog);
	}

	//Shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//Print Linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		LOG(LOG_TYPE::ERRO, "ERROR::SHADER::PROGRAM::LINKING_FAILED\N %s", infoLog);
	}
	else
	{
		LOG(LOG_TYPE::SUCCESS, "SUCCESS: Shader '%s' is compiled!", name.c_str());
	}

	//Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetMat4(const std::string& name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::SetVec3(const std::string& name, const float* value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void Shader::SetVec2(const std::string& name, const float* value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}