#include "Shader.h"

#include "Glew/include/glew.h"

#include "Globals.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath, std::string name)
{
	this->name = name;

	RetriveShader(vertexPath, fragmentPath);
	CompileShader();
}

void Shader::RetriveShader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vShaderFile; //Vertex
	std::ifstream fShaderFile; //Fragment

	//Ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//Open File
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		//Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//Close file handlers
		vShaderFile.close();
		fShaderFile.close();

		//Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
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